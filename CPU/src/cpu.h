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

	t_log* cpu_logger;

// ------------------------------------------------------------------------------------------
// -- Variables del archivo de configuración --
// ------------------------------------------------------------------------------------------

	t_config* cpu_config;
	char *tam_max_segmentos, *retardo_instruccion;
	char *ip_memoria, *puerto_memoria, *puerto_escucha;


// ------------------------------------------------------------------------------------------
// -- Server del proceso --
// ------------------------------------------------------------------------------------------

	//uint32_t handshake;
	int server_cpu;
	int socket_memoria;
	uint32_t respuesta;
	pthread_t atender_kernel;
	pthread_t conexionMemoria;

	void* abrirSocketKernel();
	void* conectarMemoria();

