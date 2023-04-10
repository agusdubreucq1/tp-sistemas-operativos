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
#include <commons/log.h>

// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------
	t_log* kernel_logger;
	t_config* kernel_config;
	u_int32_t grado_maximo_multiprogramacion;
	char *ip_memoria, *puerto_memoria, *ip_filesystem, *puerto_filesystem, *ip_cpu;
	char *puerto_cpu, *puerto_escucha, *estimacion_inicial, *hrrn_alfa;
	char *algoritmo_planificacion, **recursos, **instancias_recursos;


#endif /* KERNEL_H_ */
