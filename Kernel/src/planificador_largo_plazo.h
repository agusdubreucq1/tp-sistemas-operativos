/*
 * planificacion.h
 *
 *  Created on: Apr 25, 2023
 *      Author: utnso
 */

#ifndef PLANIFICADOR_LARGO_PLAZO_H_
#define PLANIFICADOR_LARGO_PLAZO_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <commons/string.h>
#include <utils/datos.h>
#include <utils/pcb.h>
#include <pthread.h>
#include <utils/utils.h>
#include <semaphore.h>

// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------

	extern t_log* kernel_logger;
	extern char* algoritmo_planificacion;

// ------------------------------------------------------------------------------------------
// -- Estructuras Planificacion --
// ------------------------------------------------------------------------------------------

	extern t_list* lista_new;
	//extern sem_t semaforo_multiprogramacion;
	//extern sem_t cantidad_procesos_new;
	//extern sem_t cantidad_procesos_ready;

// ------------------------------------------------------------------------------------------
// -- Funciones --
// ------------------------------------------------------------------------------------------

	void ingresar_en_lista(t_pcb* pcb, t_list* lista, char* modulo, pthread_mutex_t* semaforo_mutex, sem_t* semaforo, estado_code estado);
	void log_cambiar_estado(int pid, estado_code viejo, estado_code nuevo);

#endif /* PLANIFICADOR_LARGO_PLAZO_H_ */
