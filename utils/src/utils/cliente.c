/*
 * cliente.c
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#include "cliente.h"

int crear_conexion(char *ip, char* puerto, t_log* logger, char* modulo){
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	// Ahora vamos a crear el socket.
	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	int resultado_conexion = connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

	while(resultado_conexion == -1){
		log_info(logger, "Hubo un fallo al conectarse al modulo %s %s:%s, reintentando...", modulo, ip, puerto);
		sleep(1);
		resultado_conexion = connect(socket_cliente, server_info -> ai_addr, server_info -> ai_addrlen );
	}

	//log_info(logger, "Conexion establecida con %s : %s:%s, reintentando...", modulo, ip, puerto);
	freeaddrinfo(server_info);

	return socket_cliente;
}

void* serializar_paquete(t_paquete* paquete, int bytes){
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}


uint32_t handshake(int conexion, uint32_t envio, t_log* logger, char *modulo){
	uint32_t result;

	send(conexion, &envio, sizeof(uint32_t), 0);
	recv(conexion, &result, sizeof(uint32_t), MSG_WAITALL);

	if(result == 0) {
		log_info(logger, "Conexion establecida con %s", modulo);
	} else {
		log_error(logger, "Error en la conexión con %s", modulo);
		return -1;
	}

	return result;
}

void enviar_mensaje(char* mensaje, int socket_cliente){
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}

void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}


t_paquete* crear_paquete(void)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = PAQUETE;
	crear_buffer(paquete);
	return paquete;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete* paquete, int socket_cliente, t_log* logger, char* modulo)
{
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	log_info(logger,"Datos enviados, esperando respuesta de %s...", modulo);

	uint32_t respuesta;
	recv(socket_cliente, &respuesta, sizeof(uint32_t), MSG_WAITALL);

	if(respuesta == bytes){
		log_info(logger,"Datos enviados correctamente");
	}
	else {
		log_error(logger,"Han llegado menos bytes que los enviados por %s", modulo);
	}

	free(a_enviar);
}

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}

/*t_buffer* buffer_serializar_uint32_t(uint32_t* it) {
    uint32_t size = sizeof(uint32_t);
    void* stream = malloc(size);
    memcpy(stream, it, size);
    return buffer_create_for(size, stream);
}


void buffer_agregar_uint32_t(t_buffer* buffer, uint32_t* it) {
	t_buffer* buf = buffer_serializar_uint32_t(it);
	buffer->stream = realloc(buffer->stream, buffer->size + buf->size);
	memcpy(buffer->stream + buffer->size, buf->stream, buf->size);
	buffer->size += buf->size;
	buffer_destroy(&buf);
}*/

