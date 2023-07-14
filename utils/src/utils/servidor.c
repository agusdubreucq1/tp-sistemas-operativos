/*
 * servidor.c
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#include "servidor.h"

int iniciar_servidor(char* ip, char* puerto, t_log* logger){
	int socket_servidor;

	struct addrinfo hints, *servinfo;//, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(ip, puerto, &hints, &servinfo);

    socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

    bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);

	listen(socket_servidor, SOMAXCONN);

    freeaddrinfo(servinfo);

    log_trace(logger, "Listo para escuchar a mi cliente");

    return socket_servidor;
}

int esperar_cliente(int socket_servidor, t_log* logger){
	//struct sockaddr_in dir_cliente;
	//int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, NULL, NULL);

	log_trace(logger, "Se conecto un cliente!");

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
	buffer = malloc(*size);           //liberado en recibir instrucciones
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

char* recibirMensaje(int socket_cliente, t_log* logger){
	recibir_operacion(socket_cliente);
	return recibir_instruccion(socket_cliente, logger);
}

void recibir_mensaje(int socket_cliente, t_log* logger)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_trace(logger, "Me llego el mensaje: %s", buffer);
	free(buffer);
}

char* recibir_instruccion(int socket_cliente, t_log* logger)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_trace(logger, "Recibi Instruccion: %s", buffer);
	return buffer;
}

void cerrar_consola(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	free(buffer);
}

char* recibir_instruccion_cpu(int socket_cliente, t_log* logger)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Instruccion de CPU %s", buffer);
	return buffer;
}



t_list* recibir_instrucciones(int socket_cliente, uint32_t* tamanio_recibido){
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);

	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);

		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;

		char *token = strtok(valor, "\r\n");
		while (token != NULL){
			char* token_copia = strdup(token);
			//printf("token: %s, size token: %ld\n", token, sizeof(*token));
			//printf("token[ult]: %c , token[ult+1]: %c \n", token[strlen(token)-1], token[strlen(token)]);
			list_add(valores, token_copia);
			token = strtok(NULL, "\r\n");
		}
		free(valor);
	}

	*tamanio_recibido = desplazamiento + sizeof(uint32_t) * 2;
	send(socket_cliente, tamanio_recibido, sizeof(uint32_t), 0);
	free(buffer);

	return valores;
}



void cerrar_conexion(int socket_cliente) {
    close(socket_cliente);
    socket_cliente = -1;
}
