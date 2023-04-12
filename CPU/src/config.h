#ifndef CONFIG
#define CONFIG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>
#include "logger.h"

// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------
	extern t_log* cpu_logger;

// ------------------------------------------------------------------------------------------
// -- Variables del archivo de configuración --
// ------------------------------------------------------------------------------------------

	extern char *retardo_instruccion, *tam_max_segmentos;
	extern char *ip_memoria, *puerto_memoria, *puerto_escucha;


// --------------------------------------

t_config* iniciar_config(char*);
void leer_configs(t_config* cpu_config, t_log* cpu_logger);
void loggear_configs(t_log* cpu_logger);

#endif
