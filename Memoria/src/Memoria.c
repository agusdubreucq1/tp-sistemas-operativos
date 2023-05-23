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

	pthread_create(&hilo_conexion_Kernel, NULL, atenderKernel, NULL);
	pthread_detach(hilo_conexion_Kernel);

	pthread_create(&hilo_conexion_CPU, NULL, atenderCPU, NULL);
	pthread_detach(hilo_conexion_CPU);

	pthread_create(&hilo_conexion_FileSystem, NULL, atenderFileSystem, NULL);
	pthread_join(hilo_conexion_FileSystem, NULL);


	return EXIT_SUCCESS;
}

void* atenderKernel(){
	int socket_kernel = abrir_socket();

	while(1){
		int cod_op = recibir_operacion(socket_kernel);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(socket_kernel, memoria_logger);
			break;
		}
	}
	return "";
}


void* atenderCPU(){
	int socket_CPU = abrir_socket();

	while(1){
		int cod_op = recibir_operacion(socket_CPU);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(socket_CPU, memoria_logger);
			break;
		}
	}
	return "";
}

void* atenderFileSystem(){
	int socket_FileSystem = abrir_socket();

	while(1){
		int cod_op = recibir_operacion(socket_FileSystem);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(socket_FileSystem, memoria_logger);
			break;
		}
	}
	return "";
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


void cerrar_conexiones(){
	printf("\ncerrando conexiones\n");

	close(server_memoria);
	//close(socket_Kernel);
	printf("cerre conexiones");
	exit(1);
}

