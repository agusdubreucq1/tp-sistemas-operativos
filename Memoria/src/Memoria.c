#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "Memoria.h"


int main(void) {

	//signal(SIGINT, cerrar_conexiones);

	memoria_logger = iniciar_logger("../../logs/logMemoria.log", "Memoria");

	if (memoria_logger == NULL){
		exit(1);
	}

	memoria_config = iniciar_config("../../config/Memoria.config", "Memoria");

	if (memoria_config == NULL){
		exit(2);
	}

	leer_configs(memoria_config, memoria_logger);
	log_info(memoria_logger, "¡Memoria iniciado correctamente!");

	server_memoria = iniciar_servidor(NULL, puerto_escucha, memoria_logger);
	log_info(memoria_logger, "Servidor listo para recibir al cliente");

	sem_init(&sem_conexiones, 0, 0);
	sem_init(&sem_kernel, 0, 0);
	sem_init(&sem_cpu, 0, 0);
	pthread_mutex_init(&sem_execute_fileSystem, NULL);

	pthread_create(&hilo_estructuras, NULL, (void *) crear_estructuras, NULL);
	pthread_detach(hilo_estructuras);

	pthread_create(&hilo_conexion_FileSystem, NULL, atenderFileSystem, NULL);
	pthread_detach(hilo_conexion_FileSystem);

	pthread_create(&hilo_conexion_CPU, NULL, atenderCPU, NULL);
	pthread_detach(hilo_conexion_CPU);

	pthread_create(&hilo_conexion_Kernel, NULL, atenderKernel, NULL);
	pthread_join(hilo_conexion_Kernel, NULL);

	return EXIT_SUCCESS;
}

void* atenderKernel(){
	sem_wait(&sem_kernel);
	socket_kernel = abrir_socket();
	sem_post(&sem_conexiones);

	while(1){
		int cod_op = recibir_operacion(socket_kernel);
		switch (cod_op) {
		case MENSAJE:
			char* recibi = recibir_instruccion(socket_kernel, memoria_logger);
			ejecutar_instruccion(recibi);
			free(recibi);
			break;
		case PAQUETE:
			int size;
			void* buffer;
			int* tam_recibido= malloc(sizeof(int));
			tam_recibido = 0;
			buffer = recibir_buffer(&size, socket_kernel);

			tabla_recibida = deserializar_segmentos(buffer, tam_recibido);
			log_info(memoria_logger, "Recibi Tabla de Segmentos - PID: %d", tabla_recibida->pid);

			*tam_recibido+=2*sizeof(int);
			send(socket_kernel, tam_recibido, sizeof(int), 0);
			free(buffer);
			break;
		}
	}
	return "";
}

void* atenderCPU(){
	sem_wait(&sem_cpu);
	socket_cpu = abrir_socket();
	sem_post(&sem_kernel);

	while(1){
		int cod_op = recibir_operacion(socket_cpu);
		switch (cod_op) {
		case MENSAJE:
			char* motivo = recibir_instruccion(socket_cpu, memoria_logger);
			ejecutar_instruccion(motivo);
			free(motivo);
			break;
		}
	}
	return "";
}

void* atenderFileSystem(){

	socket_filesystem = abrir_socket();
	sem_post(&sem_cpu);

	while(1){
		int cod_op = recibir_operacion(socket_filesystem);
		switch (cod_op) {
		case MENSAJE:
			//printf("\nRECIBI MENSJAE DE FILESYSTEM\n\n");
			char* instruccion = recibir_instruccion(socket_filesystem, memoria_logger);
			pthread_mutex_lock(&sem_execute_fileSystem);
			ejecutar_instruccion(instruccion);
			pthread_mutex_unlock(&sem_execute_fileSystem);
			free(instruccion);
			break;
		}
	}
	return "";
}


void ejecutar_instruccion(char* motivo){
	char** parametros = string_split(motivo, " ");
	codigo_instruccion cod_instruccion = obtener_codigo_instruccion(parametros[0]);
	//printf("\nMEMORIA LIBRE %i\n", memoria_libre);

	switch(cod_instruccion) {
	case INICIAR:
		parametros = string_split(motivo, " ");
		t_tabla_segmentos* tabla = crear_tabla(atoi(parametros[1]));
		enviar_segmentos(tabla, socket_kernel);
		break;
	case CREATE_SEGMENT:
		parametros = string_split(motivo, " ");
		char* mensaje = elegir_hueco(atoi(parametros[2]));

		//char* mensaje = "OUT";
		//imprimir_huecos(lista_huecos);
		if(!(strcmp(mensaje, "OUT"))){
			log_error(memoria_logger, "Out of memory - Cerrando PID: %s", parametros[3]);
			//enviar_mensaje("OUT", socket_kernel);
			log_info(memoria_logger, "Eliminación de Proceso PID: %s", parametros[3]);
			t_tabla_segmentos* tabla_a_borrar = buscar_tabla_proceso(atoi(parametros[3]));
			borrar_tabla(tabla_a_borrar);
			list_remove_element(tablas_segmentos, tabla_a_borrar);
			enviar_mensaje("OUT", socket_kernel);
			//list_destroy_and_destroy_elements(tabla_a_borrar->segmentos, liberar_elemento_list);
			free(tabla_a_borrar);
		} else if(!(strcmp(mensaje, "COMPACT"))){
			//pthread_mutex_lock(&sem_execute_fileSystem);
			enviar_mensaje("COMPACT", socket_kernel);
			//pthread_mutex_unlock(&sem_execute_fileSystem);
		} else{
			void* base_elegida = (void*) strtoul(mensaje, NULL, 16);
			void* limite_elegido = (void*) (base_elegida + atoi(parametros[2]));

			log_info(memoria_logger, "PID: %s - Crear Segmento: %s - Base: %p - TAMAÑO: %s", parametros[3], parametros[1], base_elegida, parametros[2]);
			t_tabla_segmentos* tabla_buscada = buscar_tabla_proceso(atoi(parametros[3]));
			t_segmento* segmento_nuevo = list_get(tabla_buscada->segmentos, atoi(parametros[1]));
			segmento_nuevo->direccion_base = base_elegida;
			segmento_nuevo->limite = limite_elegido;
			segmento_nuevo->libre = 0;
			memoria_libre -= atoi(parametros[2]);
			//imprimir_segmentos(tabla_buscada);

			char motivo[30] = "SEGMENT ";
			char numero[20];
			sprintf(numero, "%p", base_elegida);
			strcat(motivo, numero);

			enviar_mensaje(motivo, socket_kernel);
		}
		//free(mensaje);
		break;
	case DELETE_SEGMENT:
		parametros = string_split(motivo, " ");
		t_tabla_segmentos* tabla_del_segmento = buscar_tabla_proceso(atoi(parametros[2]));
		t_segmento* segmento_a_borrar = list_get(tabla_del_segmento->segmentos, atoi(parametros[1]));
		int size = segmento_a_borrar->limite - segmento_a_borrar->direccion_base;
		log_info(memoria_logger, "PID: %s - Eliminar Segmento: %s - Base: %p - TAMAÑO: %u", parametros[2], parametros[1], segmento_a_borrar->direccion_base, size);
		borrar_segmento(segmento_a_borrar);
		//imprimir_huecos(lista_huecos);
		enviar_segmentos(tabla_del_segmento, socket_kernel);
		break;
	case FINALIZAR:
		parametros = string_split(motivo, " ");
		t_tabla_segmentos* tabla_a_finalizar = buscar_tabla_proceso(atoi(parametros[1]));
		borrar_tabla(tabla_a_finalizar);
		list_remove_element(tablas_segmentos, tabla_a_finalizar);
		//list_destroy_and_destroy_elements(tabla_a_finalizar->segmentos, liberar_elemento_list);
		free(tabla_a_finalizar);
		log_info(memoria_logger, "Eliminación de Proceso PID: %s", parametros[1]);
		break;
	case MOV_IN:

		parametros = string_split(motivo, " ");
		void *direccion_fisica_mov_in = (void *)strtoul(parametros[1], NULL, 16);
		int tamanio_mov_in = atoi(parametros[2]);
		char *valor_registro = malloc(tamanio_mov_in + 1);
		memcpy(valor_registro, direccion_fisica_mov_in, tamanio_mov_in);
		usleep(atoi(retardo_memoria) * 1000);
		valor_registro[tamanio_mov_in] = '\0';
		//printf("\nValor_registro: %s", valor_registro);
		enviar_mensaje(valor_registro, socket_cpu);
		free(valor_registro);
		break;

	case MOV_OUT:

		parametros = string_split(motivo, " ");
		void *direccion_fisica_mov_out = (void *)strtoul(parametros[1], NULL, 16);
		char *valor = (char *)parametros[2];
		int tamanio_mov_out = atoi(parametros[3]);
		memcpy(direccion_fisica_mov_out, valor, tamanio_mov_out);
		usleep(atoi(retardo_memoria) * 1000);
		//imprimir_memoria();
		break;

	case F_WRITE:
		parametros = string_split(motivo, " ");
		void *direccion_fisica_write = (void *)strtoul(parametros[1], NULL, 16);
		int tamanio_write = atoi(parametros[2]);
		char *valor_registro_write = malloc(tamanio_write + 1);
		memcpy(valor_registro_write, direccion_fisica_write, tamanio_write);
		usleep(atoi(retardo_memoria) * 1000);
		valor_registro_write[tamanio_write] = '\0';
		//printf("\nValor_registro: %s", valor_registro_write);
		enviar_mensaje(valor_registro_write, socket_filesystem);
		free(valor_registro_write);

		break;
	case F_READ:

		break;
	case COMPACT:
		pthread_mutex_lock(&sem_execute_fileSystem);
		compactar_perrito();
		usleep(atoi(retardo_compactacion)*1000);
		pthread_mutex_unlock(&sem_execute_fileSystem);
		int elementos = list_size(tablas_segmentos);
		//printf("\n ELEMENTOS %i \n", elementos);
		for (int i = 0; i < elementos; i++){
			t_tabla_segmentos* tabla = list_get(tablas_segmentos, i);
			enviar_segmentos(tabla, socket_kernel);
		}
		break;
	default:
		break;
	}
}


int abrir_socket(){
	int socket = esperar_cliente(server_memoria, memoria_logger);

	uint32_t resultOk = 0;
	uint32_t resultError = -1;

	recv(socket, &respuesta, sizeof(uint32_t), MSG_WAITALL);
	if(respuesta == 1)
	   send(socket, &resultOk, sizeof(uint32_t), 0);
	else
	   send(socket, &resultError, sizeof(uint32_t), 0);

	return socket;
}


void crear_estructuras(){
	sem_wait(&sem_conexiones);

	memoria_fisica = reservar_espacio_memoria();
	memoria_libre = atoi(tam_memoria);
	log_info(memoria_logger, "Espacio reservado: %s Bytes -> Direccion: %p", tam_memoria, memoria_fisica);
	algoritmo = obtener_algoritmo_asignacion(algoritmo_asignacion);
	tablas_segmentos = list_create();
	lista_huecos = list_create();
	hueco_inicial = crear_segmento(memoria_fisica, memoria_fisica + memoria_libre, 0, 1, 0);
	list_add(lista_huecos, hueco_inicial);

	char* base_elegida = elegir_hueco(atoi(tam_segmento_0));
	void* base = (void*) strtoul(base_elegida, NULL, 16);
	segmento_cero = crear_segmento(base, base + atoi(tam_segmento_0), 0, 0, 0);
}

void cerrar_conexiones(){
	//printf("\ncerrando conexiones\n");

	close(server_memoria);
	//close(socket_Kernel);
	//printf("cerre conexiones");
	exit(1);
}

void liberar_elemento_list(void* elemento){
	free(elemento);
}

