/*
 * server.c
 *
 *  Created on: Apr 12, 2023
 *      Author: utnso
 */

#include "server.h"

int iniciar_servidor(void){
	int socket_servidor;

	struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(IP, puerto_escucha, &hints, &servinfo);

    socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

    bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);

	listen(socket_servidor, SOMAXCONN);

    freeaddrinfo(servinfo);

    log_trace(fileSystem_logger, "Listo para escuchar a mi cliente");

    return socket_servidor;
}

int esperar_cliente(int socket_servidor){
	struct sockaddr_in dir_cliente;
	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, NULL, NULL);

	log_info(fileSystem_logger, "Se conecto un cliente!");

	return socket_cliente;
}

int recibir_operacion(int socket_cliente){
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) != 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente){
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente){
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(fileSystem_logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}




