#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "logger.h"
#include "config.h"
#include "server.h"
#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <pthread.h>

// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------

	t_log* fileSystem_logger;

// ------------------------------------------------------------------------------------------
// -- Config del proceso --
// ------------------------------------------------------------------------------------------

	t_config* fileSystem_config;

	char *ip_memoria, *puerto_memoria, *puerto_escucha, *path_superbloque;
	char *path_bitmap, *path_bloques, *path_fcb, *retardo_acceso_bloque;

// ------------------------------------------------------------------------------------------
// -- Server del proceso --
// ------------------------------------------------------------------------------------------

	//uint32_t handshake;
	int server_fileSystem;
	int socket_memoria;
	uint32_t respuesta;
	pthread_t atender_kernel;
	pthread_t conexionMemoria;

	void* abrirSocketKernel();
	void* conectarMemoria();

#endif /* FILESYSTEM_H_ */
