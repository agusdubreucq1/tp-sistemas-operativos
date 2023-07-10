#ifndef MEMORIA_H_
#define MEMORIA_H_

#include "config.h"
#include "espacio_de_memoria.h"
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
#include <utils/instruccion.h>
#include "tablas_segmento.h"
#include "bitmap_memoria.h"
#include <commons/bitarray.h>
#include <utils/deserializar.h>

#define IP_SERVER "127.0.0.1"

// ------------------------------------------------------------------------------------------
// -- Variables Logger --
// ------------------------------------------------------------------------------------------

	t_log* memoria_logger;

// ------------------------------------------------------------------------------------------
// -- Variables Config --
// ------------------------------------------------------------------------------------------

	t_config* memoria_config;
	char *algoritmo_asignacion, *puerto_escucha, *tam_memoria, *tam_segmento_0, *cant_segmentos;
	char *retardo_memoria, *retardo_compactacion;

// ------------------------------------------------------------------------------------------
// -- Variables Socket --
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

	sem_t sem_conexiones;
	sem_t sem_kernel;
	sem_t sem_cpu;

	pthread_mutex_t sem_execute_fileSystem;

// ------------------------------------------------------------------------------------------
// -- Variables --
// ------------------------------------------------------------------------------------------

	t_list* tablas_segmentos;
	t_list* lista_huecos;
	t_list* tabla_segmentos;
	t_segmento* segmento_cero;
	t_segmento* hueco_inicial;
	void* memoria_fisica;
	t_tabla_segmentos* tabla_recibida;
	t_bitarray* bitmap;
	int memoria_libre;
	alg_asignacion algoritmo;

// ------------------------------------------------------------------------------------------
// -- Funciones --
// ------------------------------------------------------------------------------------------

	void* atenderKernel();
	void* atenderCPU();
	void* atenderFileSystem();
	int abrir_socket();
	void cerrar_conexiones();
	void crear_estructuras();
	void ejecutar_instruccion(char* motivo);
	void liberar_elemento_list(void* elemento);

#endif
