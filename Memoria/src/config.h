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

	extern t_log* memoria_logger;

// ------------------------------------------------------------------------------------------
// -- Variables del archivo de configuración --
// ------------------------------------------------------------------------------------------

	extern char *algoritmo_asignacion, *puerto_escucha, *tam_memoria;
	extern char *tam_segmento_0, *cant_segmentos, *retardo_memoria, *retardo_compactacion;

// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

	void leer_configs(t_config*, t_log*);
	void loggear_configs(t_log*);


#endif
