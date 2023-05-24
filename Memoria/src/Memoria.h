#ifndef MEMORIA_H_
#define MEMORIA_H_

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <pthread.h>
#include <utils/utils.h>
#include <utils/servidor.h>
#include <utils/cliente.h>
#include <utils/datos.h>
#include <semaphore.h>

#define IP_SERVER "127.0.0.1"

// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------

	t_log* memoria_logger;

// ------------------------------------------------------------------------------------------
// -- Config del proceso --
// ------------------------------------------------------------------------------------------

	t_config* memoria_config;
	char *algoritmo_asignacion, *puerto_escucha, *tam_memoria, *tam_segmento_0, *cant_segmentos;
	char *retardo_memoria, *retardo_compactacion;

// ------------------------------------------------------------------------------------------
// -- Socket del proceso --
// ------------------------------------------------------------------------------------------

	pthread_t hilo_conexion_Kernel;
	pthread_t hilo_conexion_CPU;
	pthread_t hilo_conexion_FileSystem;
	pthread_t hilo_estructuras;
	uint32_t respuesta;
	int server_memoria;
	int socket_kernel;
	int socket_cpu;
	int socket_filesystem;

	sem_t semaforo_conexiones;

// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

	void* atenderKernel();
	void* atenderCPU();
	void* atenderFileSystem();
	int abrir_socket();
	void cerrar_conexiones();
	void crear_estructuras();

#endif
