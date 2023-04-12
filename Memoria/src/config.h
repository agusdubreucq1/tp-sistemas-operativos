#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>

#include "logger.h"


   extern t_config* memoria_config;
   extern t_log* memoria_logger;

   extern int puerto_escucha, tam_memoria, tam_segmento_0, cant_segmentos, retardo_memoria, retardo_compactacion;
   extern char* algoritmo_asignacion;

t_config* iniciar_config(char*);
void leer_configs(t_config*, t_log*);
void loggear_configs(t_log*);
