#include "fileSystem.h"


int main(void){

	fileSystem_logger = iniciar_logger("../../logs/logFileSystem.log", "FileSystem");

	if (fileSystem_logger == NULL){
		exit(1);
	}

	fileSystem_config = iniciar_config("../../config/FileSystem.config", "FileSystem");

	if (fileSystem_config == NULL){
		exit(2);
	}

	leer_configs(fileSystem_config, fileSystem_logger);
	log_info(fileSystem_logger, "¡File System iniciado correctamente!");

	server_fileSystem = iniciar_servidor(IP_SERVER, puerto_escucha, fileSystem_logger);
	log_info(fileSystem_logger, "Servidor listo para recibir al cliente");

	inicializar_estructuras();

	pthread_create(&conexionMemoria, NULL, conectarMemoria, NULL);
	pthread_detach(conexionMemoria);
	pthread_create(&atender_kernel, NULL, abrirSocketKernel, NULL);
	pthread_join(atender_kernel, NULL);

	return EXIT_SUCCESS;
}

void* abrirSocketKernel(){
	socket_Kernel = esperar_cliente(server_fileSystem, fileSystem_logger);

	uint32_t resultOk = 0;
	uint32_t resultError = -1;

	recv(socket_Kernel, &respuesta, sizeof(uint32_t), MSG_WAITALL);
	if(respuesta == 1)
	   send(socket_Kernel, &resultOk, sizeof(uint32_t), 0);
	else
	   send(socket_Kernel, &resultError, sizeof(uint32_t), 0);

	int cod_op = recibir_operacion(socket_Kernel);
	switch (cod_op) {
	case MENSAJE:
		recibir_mensaje(socket_Kernel, fileSystem_logger);
		break;
	}

	while(1){
		recibir_mensaje_kernel();
	}

	return "";
}

void* conectarMemoria(){
	socket_memoria = crear_conexion(ip_memoria, puerto_memoria, fileSystem_logger, "Memoria");
	handshake(socket_memoria, 1, fileSystem_logger, "Memoria");

	enviar_mensaje("Conectado al FileSystem", socket_memoria);

	sem_post(&espera_cerrar);

	while(1){
		recibir_mensaje_memoria();
	}

	return "";
}

void recibir_mensaje_kernel(){
	int cod_op;
	cod_op = recibir_operacion(socket_Kernel);
	switch (cod_op) {
		case MENSAJE:
			char* mensaje = string_new();
			char* instruccion = recibir_instruccion(socket_Kernel, fileSystem_logger);
			string_append(&mensaje, instruccion);
			ejecutar_instruccion(mensaje);
			free(mensaje);
			free(instruccion);
			break;
		default: break;
	}
}

void recibir_mensaje_memoria(){
	int cod_op;
	cod_op = recibir_operacion(socket_memoria);
	switch (cod_op) {
		case MENSAJE:
			char* mensaje = string_new();
			char* instruccion = recibir_instruccion(socket_memoria, fileSystem_logger);
			string_append(&mensaje,instruccion);
			//ejecutar
			free(mensaje);
			free(instruccion);
			break;
		default: break;
	}
}

void inicializar_estructuras(){

	levantar_superBloque(path_superbloque, fileSystem_logger);
	inicializar_bitmap();
	inicializar_bloques();
	inicializar_FCBs();

	printf("todo creado\n");
}


int existe_archivo(char* nombre){
	for(int i=0; i<list_size(lista_fcb);i++){
		t_fcb* fcb= list_get(lista_fcb, i);
		if(string_equals_ignore_case(fcb->nombre_archivo, nombre)){
			return 1;
		}
	}
	return 0;
}

void crear_archivo(char* nombre){
	t_fcb* nuevo_fcb = crear_fcb(nombre);
	list_add(lista_fcb, nuevo_fcb);
}

void cambiar_tamanio(char* archivo, int tamanio){
	t_fcb* fcb = fcb_segun_nombre(archivo);
	int bloques_asignados = bloques_necesarios(fcb->tamano_archivo);
	if(tamanio > fcb->tamano_archivo){
		int bloques_a_agregar = bloques_necesarios(tamanio)-bloques_asignados;
		for(int i=1; i<=bloques_a_agregar; i++){
			if(bloques_asignados == 0){//si no tiene asignado un puntero directo
				fcb->puntero_directo = asignar_bloque();
				printf("\nse asigna el bloque: %d como puntero directo\n\n", fcb->puntero_directo);
			}else{
				if(bloques_asignados<2){//si no tiene asignado un puntero indirecto
					fcb->puntero_indirecto = asignar_bloque();
					printf("\nse asigna el bloque %d como el puntero indirecto\n\n", fcb->puntero_indirecto);
				}
				uint32_t bloque_asignado = asignar_bloque();
				int posicion = fcb->puntero_indirecto* tamanio_bloque + (bloques_asignados - 1)*4;//en el bloque de punteros, posicion segun los bloques ya asignados
				printf("\nposicion: %d\nbloque_asignado: %d\n", posicion, bloque_asignado);
				escribirArchivoBloques((void*)&bloque_asignado, posicion, sizeof(uint32_t));
			}
			bloques_asignados+=1;
		}
		leerBloque(fcb->puntero_indirecto);

	}else{//liberar el puntero indirectoooo
		int bloques_a_liberar = bloques_asignados - bloques_necesarios(tamanio);
		printf("\n bloques a liberar: %d\n", bloques_a_liberar);
		for(int i=0;i<bloques_a_liberar;i++){
			if(bloques_asignados==1){
				printf("liberando PD: %d\n", fcb->puntero_directo);
				bitarray_clean_bit(bitmap, fcb->puntero_directo);
			}else{
				int posicion = fcb->puntero_indirecto* tamanio_bloque + (bloques_asignados - 2)*4;
				uint32_t bloque_a_liberar;
				leerArchivoBloques((void*)&bloque_a_liberar,posicion, sizeof(uint32_t));
				printf("liberando bloque: %d\n", bloque_a_liberar);
				bitarray_clean_bit(bitmap, bloque_a_liberar);
				if(bloques_asignados==2){
					printf("\nlibero el puntero indirecto: %d \n", fcb->puntero_indirecto);
					bitarray_clean_bit(bitmap, fcb->puntero_indirecto);//libero el bloque del puntero indirecto
					imprimir_bitmap_20(bitmap);
				}
			}
			bloques_asignados-=1;
		}

	}
	fcb->tamano_archivo = tamanio;
	grabar_fcb(fcb);
	grabar_bitmap(bitmap);
	imprimir_bitmap_20(bitmap);
}



void ejecutar_instruccion(char* instruccion){
	char** parametros = string_split(instruccion, " ");
	codigo_instruccion cod_instruccion = obtener_codigo_instruccion(parametros[0]);

	switch(cod_instruccion) {
		case F_OPEN:
			parametros = string_split(instruccion, " ");
			char* archivo_abrir = parametros[1];
			printf("se solicito abrir el archivo: %s \n", archivo_abrir);
			if(existe_archivo(archivo_abrir)){
				printf("el archivo: %s ya existia\n", archivo_abrir);
				log_info(fileSystem_logger, "abrir archivo: %s",archivo_abrir);
			}else{
				crear_archivo(archivo_abrir);
				log_info(fileSystem_logger, "crear archivo: %s", archivo_abrir);
				printf("el archivo: %s fue creado", archivo_abrir);
			}
			imprimir_bitmap_20(bitmap);
			enviar_mensaje("el filesystem abrio el archivo", socket_Kernel);
			break;
		case F_TRUNCATE:
			char* archivo_truncar = parametros[1];
			int tamanio = atoi(parametros[2]);
			cambiar_tamanio(archivo_truncar, tamanio);
			log_info(fileSystem_logger, "truncar archivo: %s - tamaño: %d", archivo_truncar, tamanio);
			enviar_mensaje("el filesystem trunco el archivo", socket_Kernel);
			break;
		case F_WRITE:
			enviar_mensaje("se escribio el archivo", socket_Kernel);
			printf("\nle envie el mensaje al kernel");
			break;
		case F_READ:
			enviar_mensaje("se leyo el archivo", socket_Kernel);
			break;
		default:
			break;
	}
	string_iterate_lines(parametros, (void*) free);
	free(parametros);
}

