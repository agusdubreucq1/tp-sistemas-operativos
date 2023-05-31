#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "config.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/log.h>
#include <utils/utils.h>
#include <utils/servidor.h>
#include <utils/cliente.h>
#include <utils/datos.h>

#define IP_SERVER "127.0.0.1"

// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------

	t_log* fileSystem_logger;

// ------------------------------------------------------------------------------------------
// -- Config del proceso --
// ------------------------------------------------------------------------------------------

	t_config* fileSystem_config;
	char *ip_memoria, *puerto_memoria, *puerto_escucha, *path_superbloque;
	char *path_bitmap, *path_bloques, *path_fcb;
	u_int32_t retardo_acceso_bloque;

// ------------------------------------------------------------------------------------------
// -- Socket del proceso --
// ------------------------------------------------------------------------------------------

	int server_fileSystem;
	int socket_memoria;
	int socket_Kernel;
	uint32_t respuesta;
	pthread_t atender_kernel;
	pthread_t conexionMemoria;

// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

	void* abrirSocketKernel();
	void* conectarMemoria();
	void recibir_mensaje_kernel();

	typedef struct
	{
		int tamanio_bloque;
		int cant_bloques;
	}t_superBloque;

	t_superBloque* levantar_superBloque(char*);



#endif /* FILESYSTEM_H_ */
