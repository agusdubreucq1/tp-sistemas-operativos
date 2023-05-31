/*
 * cliente.h
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#ifndef UTILS_CLIENTE_H_
#define UTILS_CLIENTE_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <commons/log.h>
#include "datos.h"

// ------------------------------------------------------------------------------------------
// -- Funciones del cliente --
// ------------------------------------------------------------------------------------------

	int crear_conexion(char *ip, char* puerto, t_log* logger, char* modulo);
	void* serializar_paquete(t_paquete* paquete, int bytes);
	uint32_t handshake(int conexion, uint32_t envio, t_log* logger, char *modulo);
	void enviar_mensaje(char* mensaje, int socket_cliente);
	void crear_buffer(t_paquete* paquete);
	t_paquete* crear_paquete(void);
	void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
	void enviar_paquete(t_paquete* paquete, int socket_cliente, t_log* logger, char* modulo);
	void eliminar_paquete(t_paquete* paquete);
	void liberar_conexion(int socket_cliente, t_log* logger);
	void enviar_instruccion(char* mensaje, int socket_cliente);


#endif /* UTILS_CLIENTE_H_ */
