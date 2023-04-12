#ifndef MEMORIA_H_
#define MEMORIA_H_

#include "config.h"
#include "logger.h"
#include <commons/log.h>


t_config* memoria_config;
t_log* memoria_logger;

int puerto_escucha, tam_memoria, tam_segmento_0, cant_segmentos, retardo_memoria, retardo_compactacion;
char* algoritmo_asignacion;

//void leer_config(t_config*, t_log*);

#endif
