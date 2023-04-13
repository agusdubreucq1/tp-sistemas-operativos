#ifndef CONFIG
#define CONFIG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include "logger.h"

// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------

	extern t_log* fileSystem_logger;

// ------------------------------------------------------------------------------------------
// -- Config del proceso --
// ------------------------------------------------------------------------------------------

	extern t_config* fileSystem_config;
	extern char *ip_memoria, *puerto_memoria, *puerto_escucha, *path_superbloque;
	extern char *path_bitmap, *path_bloques, *path_fcb, *retardo_acceso_bloque;

// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

t_config* iniciar_config(char* kernel_config);
void leer_configs(t_config* kernel_config, t_log* fileSystem_logger);
void loggear_configs(t_log* file_logger);

#endif /* CONFIG_H_ */
