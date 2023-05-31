#ifndef CONFIG
#define CONFIG

#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/log.h>
#include <commons/config.h>

// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------

	extern t_log* fileSystem_logger;

// ------------------------------------------------------------------------------------------
// -- Config del proceso --
// ------------------------------------------------------------------------------------------

	extern char *ip_memoria, *puerto_memoria, *puerto_escucha, *path_superbloque;
	extern char *path_bitmap, *path_bloques, *path_fcb;
	extern u_int32_t retardo_acceso_bloque;
// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

	void leer_configs(t_config* kernel_config, t_log* fileSystem_logger);
	void loggear_configs(t_log* file_logger);

#endif /* CONFIG_H_ */
