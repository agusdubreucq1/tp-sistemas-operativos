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

	extern t_log* kernel_logger;

// ------------------------------------------------------------------------------------------
// -- Config del proceso --
// ------------------------------------------------------------------------------------------

	extern u_int32_t grado_maximo_multiprogramacion, estimacion_inicial;
	extern char *ip_memoria, *puerto_memoria, *ip_filesystem, *puerto_filesystem, *ip_cpu;
	extern float hrrn_alfa;
	extern char *puerto_cpu, *puerto_escucha;
	extern char *algoritmo_planificacion, **recursos, **instancias_recursos;

// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

	void leer_configs(t_config* kernel_config, t_log* kernel_logger);
	void loggear_configs(t_log*);
	void loggear_Recursos(void);

#endif /* CONFIG_H_ */


