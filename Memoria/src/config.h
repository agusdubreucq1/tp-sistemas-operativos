#ifndef CONFIG
#define CONFIG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>
#include "logger.h"

   extern t_log* memoria_logger;


   // ------------------------------------------------------------------------------------------
   // -- Variables del archivo de configuración --
   // ------------------------------------------------------------------------------------------

   //int puerto_escucha, tam_memoria, tam_segmento_0, cant_segmentos, retardo_memoria, retardo_compactacion;
   extern char *algoritmo_asignacion, *puerto_escucha, *tam_memoria;
   extern char *tam_segmento_0, *cant_segmentos, *retardo_memoria, *retardo_compactacion;

t_config* iniciar_config(char*);
void leer_configs(t_config*, t_log*);
void loggear_configs(t_log*);


#endif
