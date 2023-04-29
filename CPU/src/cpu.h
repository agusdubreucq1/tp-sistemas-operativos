#ifndef CPU_H_
#define CPU_H_


#include "config.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/log.h>
#include <utils/utils.h>
#include <utils/servidor.h>
#include <utils/cliente.h>
#include <utils/datos.h>
#include <utils/pcb.h>

#define IP_SERVER "127.0.0.1"

// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------

	t_log* cpu_logger;

// ------------------------------------------------------------------------------------------
// -- Variables del archivo de configuración --
// ------------------------------------------------------------------------------------------

	t_config* cpu_config;
	char *tam_max_segmentos, *retardo_instruccion;
	char *ip_memoria, *puerto_memoria, *puerto_escucha;


// ------------------------------------------------------------------------------------------
// -- Server del proceso --
// ------------------------------------------------------------------------------------------

	int server_cpu;
	int socket_memoria;
	int socket_Kernel;
	uint32_t respuesta;
	pthread_t atender_kernel;
	pthread_t conexionMemoria;

// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

	void* abrirSocketKernel();
	void* conectarMemoria();
	void recibir_mensaje_kernel();

#endif /* CPU_H_ */
