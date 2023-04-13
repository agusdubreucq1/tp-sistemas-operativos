#include "logger.h"
#include "config.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>

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

	uint32_t handshake;
	int server_cpu;
	void abrirSocketKernel();

