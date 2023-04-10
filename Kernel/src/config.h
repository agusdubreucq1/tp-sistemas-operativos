#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <commons/collections/queue.h>
#include <commons/collections/dictionary.h>

// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------
	extern t_log* kernel_logger;

// ------------------------------------------------------------------------------------------
// -- Variables del archivo de configuración --
// ------------------------------------------------------------------------------------------

	extern u_int32_t grado_maximo_multiprogramacion;
	extern char *ip_memoria,*puerto_memoria, *ip_filesystem, *puerto_filesystem, *ip_cpu;
	extern char *puerto_cpu, *puerto_escucha, *estimacion_inicial, *hrrn_alfa;
	extern char *algoritmo_planificacion,**recursos, **instancias_recursos;


t_config* iniciar_config(char*);
void leer_configs(t_config*, t_log*);
void loggear_configs(t_log*);


