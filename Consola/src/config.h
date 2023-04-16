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

	extern t_log* consola_logger;

// ------------------------------------------------------------------------------------------
// -- Variables del archivo de configuración --
// ------------------------------------------------------------------------------------------

	extern char *ip_kernel, *puerto_kernel;

// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

void leer_configs(t_log*, t_config*);
void loggear_valores_config(t_log*);


#endif /* CONFIG_H_ */
