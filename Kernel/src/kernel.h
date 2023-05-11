#ifndef KERNEL_H_
#define KERNEL_H_


#include "config.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/log.h>
#include <semaphore.h>
#include <utils/utils.h>
#include <utils/servidor.h>
#include <utils/cliente.h>
#include <utils/datos.h>
#include <utils/pcb.h>
#include <commons/collections/list.h>
#include "planificador_largo_plazo.h"
#include <utils/serializar.h>

#define IP_SERVER "127.0.0.1"

// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------

	t_log* kernel_logger;

// ------------------------------------------------------------------------------------------
// -- Config del proceso --
// ------------------------------------------------------------------------------------------

	t_config* kernel_config;
	u_int32_t grado_maximo_multiprogramacion;
	u_int32_t estimacion_inicial;
	char *ip_memoria, *puerto_memoria, *ip_filesystem, *puerto_filesystem, *ip_cpu;
	char *puerto_cpu, *puerto_escucha, *hrrn_alfa;
	char *algoritmo_planificacion, **recursos, **instancias_recursos;

// ------------------------------------------------------------------------------------------
// -- Socket del proceso --
// ------------------------------------------------------------------------------------------

	uint32_t respuesta;
	uint32_t resultOk;
	uint32_t resultError;
	pthread_t atender_consolas;
	pthread_t planificador_largo_plazo;
	pthread_t planificador_corto_plazo;
	pthread_t conexionFileSystem;
	pthread_t conexionCPU;
	pthread_t conexionMemoria;
	int socket_modulo;
	int server_kernel;
	int socket_fileSystem;
	int socket_cpu;
	int socket_memoria;
	kernelThreadParams* fileSystemParams;

// ------------------------------------------------------------------------------------------
// -- Estructuras Planificacion --
// ------------------------------------------------------------------------------------------

	t_list* lista_new;
	t_list* lista_ready;

	sem_t semaforo_multiprogramacion;
	sem_t cantidad_procesos_new;
	sem_t cantidad_procesos_ready;

	pthread_mutex_t semaforo_new;
	pthread_mutex_t semaforo_ready;
	pthread_mutex_t semaforo_execute;


// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

	void* conectarFileSystem();
	void* conectarCPU();
	void* conectarMemoria();
	void* recibirProcesos(int* conexion_p);
	void init_estructuras_planificacion();
	void planificarLargoPlazo();
	void planificarCortoPlazoFIFO();
	void enviar_pcb(t_pcb* pcb);
	void cerrar_conexiones();
	void recibir_mensaje_cpu(t_pcb* pcb);



#endif /* KERNEL_H_ */
