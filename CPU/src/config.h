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

	extern t_log* cpu_logger;

// ------------------------------------------------------------------------------------------
// -- Variables del archivo de configuración --
// ------------------------------------------------------------------------------------------

	extern u_int32_t retardo_instruccion;
	extern char *tam_max_segmento;
	extern char *ip_memoria, *puerto_memoria, *puerto_escucha;

// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

	void leer_configs(t_config* cpu_config, t_log* cpu_logger);
	void loggear_configs(t_log* cpu_logger);

#endif
