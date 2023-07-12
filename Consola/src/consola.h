#include "config.h"
#include "parser.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/log.h>
#include <utils/utils.h>
#include <utils/servidor.h>
#include <utils/cliente.h>
#include <utils/datos.h>

// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------

	t_log* consola_logger;

// ------------------------------------------------------------------------------------------
// -- Config del proceso --
// ------------------------------------------------------------------------------------------

	t_config* consola_config;
	char *ip_kernel, *puerto_kernel;
	//int conexion_kernel;
	//t_paquete* paquete;
	pthread_mutex_t semaforo;

// ------------------------------------------------------------------------------------------
// -- Parser --
// ------------------------------------------------------------------------------------------

	//char* buffer;

// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

	void terminar_programa(int conexion, t_log *logger, t_config* config);
