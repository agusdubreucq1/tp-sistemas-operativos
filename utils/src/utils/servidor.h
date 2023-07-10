/*
 * servidor.h
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#ifndef SRC_SERVIDOR_H_
#define SRC_SERVIDOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <string.h>
#include "datos.h"

// ------------------------------------------------------------------------------------------
// -- Funciones del servidor --
// ------------------------------------------------------------------------------------------

	int iniciar_servidor(char* ip, char* puerto, t_log* logger);
	int esperar_cliente(int socket_servidor, t_log* logger);
	int recibir_operacion(int socket_cliente);
	void* recibir_buffer(int* size, int socket_cliente);
	void recibir_mensaje(int socket_cliente, t_log* logger);
	char* recibir_instruccion_cpu(int socket_cliente, t_log* logger);
	t_list* recibir_instrucciones(int socket_cliente, uint32_t* tamanio_recibido);
	t_paquete* serializar_contexto(t_contexto_ejecucion* contexto);
	void cerrar_conexion(int socket_cliente);
	void cerrar_consola(int socket_cliente);
	char* recibir_instruccion(int socket_cliente, t_log* logger);
	char* recibirMensaje(int socket_cliente, t_log* logger);


#endif /* SRC_SERVIDOR_H_ */
