#ifndef CPU_H_
#define CPU_H_


#include "config.h"
#include "ciclo_instruccion.h"
#include "registros_cpu.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <commons/string.h>
#include <commons/log.h>
#include <utils/utils.h>
#include <utils/servidor.h>
#include <utils/cliente.h>
#include <utils/datos.h>
#include <utils/pcb.h>
#include <utils/deserializar.h>


#define IP_SERVER "127.0.0.1"

// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------

	t_log* cpu_logger;

// ------------------------------------------------------------------------------------------
// -- Variables del archivo de configuración --
// ------------------------------------------------------------------------------------------

	t_config* cpu_config;
	u_int32_t retardo_instruccion;
	char *tam_max_segmento;
	char *ip_memoria, *puerto_memoria, *puerto_escucha;

// ------------------------------------------------------------------------------------------
// -- Variables CPU
// ------------------------------------------------------------------------------------------

	t_contexto_ejecucion* contexto_de_ejecucion;
	t_tabla_segmentos* tabla_segmentos;

// ------------------------------------------------------------------------------------------
// -- Server del proceso --
// ------------------------------------------------------------------------------------------

	int server_cpu;
	int socket_Kernel;
	int socket_memoria;
	uint32_t respuesta;
	pthread_t atender_kernel;
	pthread_t conexionMemoria;

// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

	void* abrirSocketKernel();
	void* conectarMemoria();
	void recibir_mensaje_kernel();
	//void liberar_elemento_list(void* elemento);
	//void liberar_contexto(t_contexto_ejecucion* contexto_de_ejecucion);
	void cerrar_conexiones();


#endif /* CPU_H_ */
