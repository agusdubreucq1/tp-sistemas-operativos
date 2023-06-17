#include <stdio.h>
#include <stdlib.h>

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

	server_memoria = iniciar_servidor(IP_SERVER, puerto_escucha, memoria_logger);
	log_info(memoria_logger, "Servidor listo para recibir al cliente");

	sem_init(&sem_conexiones, 0, 0);
	sem_init(&sem_kernel, 0, 0);
	sem_init(&sem_cpu, 0, 0);

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
	printf("Kernel %d", socket_kernel);
	sem_post(&sem_conexiones);

	while(1){
		int cod_op = recibir_operacion(socket_kernel);
		switch (cod_op) {
		case MENSAJE:
			char* recibi = recibir_instruccion(socket_kernel, memoria_logger);
			ejecutar_instruccion(recibi);
			break;
		case PAQUETE:
			int size;
			void* buffer;
			int* tam_recibido= malloc(sizeof(int));
			tam_recibido = 0;
			buffer = recibir_buffer(&size, socket_kernel);

			tabla_recibida = deserializar_segmentos(buffer, tam_recibido);
			//t_segmento* segmento = list_get(tabla_recibida->segmentos, 0);
			log_info(memoria_logger, "Recibi Tabla de Segmentos - PID: %d", tabla_recibida->pid);

			*tam_recibido+=2*sizeof(int);
			send(socket_kernel, tam_recibido, sizeof(int), 0);
			//log_trace(memoria_logger, "Recibi contexto de ejecucion - PID: %d", contexto_de_ejecucion->pid);
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
			recibir_instruccion(socket_cpu, memoria_logger);
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
			recibir_instruccion(socket_filesystem, memoria_logger);
			break;
		}
	}
	return "";
}


void ejecutar_instruccion(char* motivo){
	char** parametros = string_split(motivo, " ");
	codigo_instruccion cod_instruccion = obtener_codigo_instruccion(parametros[0]);

	switch(cod_instruccion) {
	case INICIAR:
		parametros = string_split(motivo, " ");
		t_tabla_segmentos* tabla = crear_tabla(atoi(parametros[1]));
		enviar_segmentos(tabla, socket_kernel);
		break;
	case CREATE_SEGMENT: // LABURANDO ESTO
		parametros = string_split(motivo, " ");
		printf("\n PID: %s", parametros[3]);
		printf("\n Segmento: %s", parametros[1]);
		printf("\n Tamaño: %s", parametros[2]);
		enviar_mensaje("SEGMENT", socket_kernel);
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
	tablas_segmentos = list_create();
	inicializar_bitmap();
	segmento_cero = crear_segmento(memoria_fisica, memoria_fisica + atoi(tam_segmento_0));
}

void cerrar_conexiones(){
	printf("\ncerrando conexiones\n");

	close(server_memoria);
	//close(socket_Kernel);
	printf("cerre conexiones");
	exit(1);
}

