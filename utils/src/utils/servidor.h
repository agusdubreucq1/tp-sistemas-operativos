/*
 * servidor.h
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#ifndef SRC_SERVIDOR_H_
#define SRC_SERVIDOR_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include"datos.h"

// ------------------------------------------------------------------------------------------
// -- Funciones del servidor --
// ------------------------------------------------------------------------------------------

	int iniciar_servidor(char* ip, char* puerto, t_log* logger);
	int esperar_cliente(int socket_servidor, t_log* logger);
	int recibir_operacion(int socket_cliente);
	void* recibir_buffer(int* size, int socket_cliente);
	void recibir_mensaje(int socket_cliente, t_log* logger);
	t_list* recibir_paquete(int socket_cliente);


#endif /* SRC_SERVIDOR_H_ */
