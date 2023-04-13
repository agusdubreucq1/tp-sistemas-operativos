#ifndef KERNEL_H_
#define KERNEL_H_


#include "logger.h"
#include "config.h"
#include "client.h"
#include <commons/log.h>
#include <pthread.h>
#include "server.h"

// ------------------------------------------------------------------------------------------
// -- Socket del proceso --
// ------------------------------------------------------------------------------------------

	uint32_t respuesta;
	uint32_t resultOk;
	uint32_t resultError;
	pthread_t atender_consolas;
	pthread_t conexionFileSystem;
	pthread_t conexionCPU;
	pthread_t conexionMemoria;
	int server_kernel;
	int socket_fileSystem;
	int socket_cpu;
	int socket_memoria;


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

	void* conectarFileSystem();
	void* conectarCPU();
	void* conectarMemoria();
	void* recibirProcesos();
	void iterator(char* value);

#endif /* KERNEL_H_ */
