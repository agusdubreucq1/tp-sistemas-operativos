#ifndef CONFIG
#define CONFIG

#include "logger.h"

	extern t_log* kernel_logger;
	extern u_int32_t grado_maximo_multiprogramacion;
	extern char *ip_memoria, *puerto_memoria, *ip_filesystem, *puerto_filesystem, *ip_cpu;
	extern char *puerto_cpu, *puerto_escucha, *estimacion_inicial, *hrrn_alfa;
	extern char *algoritmo_planificacion, **recursos, **instancias_recursos;

t_config* iniciar_config(char*);
void leer_configs(t_config* kernel_config, t_log* kernel_logger);
void loggear_configs(t_log*);
void loggear_Recursos(void);

#endif /* CONFIG_H_ */


