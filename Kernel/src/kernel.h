/*
 * kernel.h
 *
 *  Created on: Apr 10, 2023
 *      Author: utnso
 */

#ifndef KERNEL_H_
#define KERNEL_H_


#include "logger.h"
#include "config.h"
#include "serverKernel.h"
#include <commons/log.h>
#include <pthread.h>

typedef struct {
	uint32_t size;
	void* stream;
} t_buffer;

	uint32_t handshake;
	uint32_t resultOk;
	uint32_t resultError;
	pthread_t atender_consolas;

	int server_kernel;
	t_buffer* buffer;

	// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------

	t_log* kernel_logger;

// ------------------------------------------------------------------------------------------
// -- Config del proceso --
// ------------------------------------------------------------------------------------------

	t_config* kernel_config;
	u_int32_t grado_maximo_multiprogramacion;
	char *ip_memoria, *puerto_memoria, *ip_filesystem, *puerto_filesystem, *ip_cpu;
	char *puerto_cpu, *puerto_escucha, *estimacion_inicial, *hrrn_alfa;
	char *algoritmo_planificacion, **recursos, **instancias_recursos;

// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

void* recibirProcesos();
void iterator(char* value);

#endif /* KERNEL_H_ */
