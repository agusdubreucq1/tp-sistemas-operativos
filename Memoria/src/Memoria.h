#ifndef MEMORIA_H_
#define MEMORIA_H_

#include "logger.h"
#include "config.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>


	t_log* memoria_logger;

// ------------------------------------------------------------------------------------------
// -- Variables del archivo de configuración --
// ------------------------------------------------------------------------------------------


	t_config* memoria_config;

	//int puerto_escucha, tam_memoria, tam_segmento_0, cant_segmentos, retardo_memoria, retardo_compactacion;
	extern char *algoritmo_asignacion, *puerto_escucha, *tam_memoria, *tam_segmento_0, *cant_segmentos;
	extern char *retardo_memoria, *retardo_compactacion;


//void leer_config(t_config*, t_log*);

uint32_t handshake;
int server_memoria;
void abrirSocketKernel();


#endif
