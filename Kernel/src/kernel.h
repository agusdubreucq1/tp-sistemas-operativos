#ifndef KERNEL_H_
#define KERNEL_H_


#include "config.h"
#include "recursos.h"
#include "archivo.h"
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
#include <utils/instruccion.h>
#include <commons/collections/list.h>
#include "planificador_largo_plazo.h"
#include <utils/serializar.h>
#include <utils/deserializar.h>
#include <sys/time.h>
#include <utils/recurso.h>
#include <utils/instruccion.h>
#include "archivo.h"
#include <string.h>
#include <commons/process.h>


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
	float hrrn_alfa;
	char *ip_memoria, *puerto_memoria, *ip_filesystem, *puerto_filesystem, *ip_cpu;
	char *puerto_cpu, *puerto_escucha;
	char *algoritmo_planificacion, **recursos, **instancias_recursos;

// ------------------------------------------------------------------------------------------
// -- Socket del proceso --
// ------------------------------------------------------------------------------------------

	//uint32_t respuesta;
	//uint32_t resultOk;
	//uint32_t resultError;
	pthread_t atender_consolas;
	pthread_t planificador_largo_plazo;
	pthread_t planificador_corto_plazo;
	pthread_t conexionFileSystem;
	pthread_t conexionCPU;
	pthread_t conexionMemoria;
	//pthread_t io_procesos;
	int socket_modulo;
	int server_kernel;
	int socket_fileSystem;
	int socket_cpu;
	int socket_memoria;
	kernelThreadParams* fileSystemParams;

	t_thread_args argumentos_hilo;

// ------------------------------------------------------------------------------------------
// -- Estructuras Planificacion --
// ------------------------------------------------------------------------------------------

	t_list* lista_new;
	t_list* lista_ready;
	t_list* lista_recursos;
	t_list* lista_archivos_abiertos;
	t_list* lista_pcbs;

	int devolver_ejecucion;
	int fileSystem_ejecutando;
	t_pcb* pcb_ejecutando;
	t_pcb* pcb_a_ejecutar;

	sem_t semaforo_multiprogramacion;
	sem_t cantidad_procesos_new;
	sem_t cantidad_procesos_ready;

	pthread_mutex_t semaforo_new;
	pthread_mutex_t semaforo_ready;
	pthread_mutex_t semaforo_execute;
	pthread_mutex_t sem_fileSystem;
	pthread_mutex_t sem_memoria;
	pthread_mutex_t sem_procesos;

	struct timeval tiempo;
	long long hora_inicio;
	int existeRecurso;
	int recibi_instruccion;
	char ultima_instruccion[30];

	t_tabla_segmentos* tablaNueva;

// ------------------------------------------------------------------------------------------
// -- Estructuras FileSystem --
// ------------------------------------------------------------------------------------------

	t_list* lista_archivos_abiertos;

// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

	void* conectarFileSystem();
	void* conectarCPU();
	void* conectarMemoria();
	void* recibirProcesos(int* conexion_p);
	void init_estructuras_planificacion();
	void planificarLargoPlazo();
	void planificarCortoPlazo();
	void enviar_pcb(t_pcb* pcb);
	void cerrar_conexiones();
	void recibir_mensaje_cpu();
	void ejecutar_segun_motivo(char* motivo);
	t_pcb* pcb_elegido_HRRN();
	void estimar_rafaga(t_pcb* pcb);
	void ejecutar_io(t_thread_args* args);
	void liberar_pcb(t_pcb* pcb);
	void liberar_elemento_list(void* elemento);
	void liberar_contexto_kernel(t_pcb* pcb);
	void imprimirSemaforos();
	char* recibir_mensaje_filesystem();
	void recibir_mensaje_memoria(char*);
	void ejecutar_motivo_memoria(char* motivo, char*);
	void accionFileSystem(t_args_fileSystem* args);
	void cerrar_archivo(char* nombre_archivo, t_pcb* pcb);
	void liberar_archivos(t_pcb* pcb);
	void mandar_a_ready(t_pcb* pcb);
	void actualizar_llegada_a_ready(t_pcb* pcb);
	void finalizar_proceso(t_pcb* pcb, char* motivo);
	void liberar_recursos(t_pcb* pcb);
	void liberarTablaSegmentos(t_pcb* pcb);
	t_pcb* buscar_pcb(t_list* lista, uint32_t pid_buscado);
	uint32_t tiempo_actual();

#endif /* KERNEL_H_ */
