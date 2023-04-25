/*
 * planificacion.c
 *
 *  Created on: Apr 25, 2023
 *      Author: utnso
 */

#include "planificador_largo_plazo.h"

void ingresar_en_lista(t_pcb* pcb, t_list* lista, char* modulo, pthread_mutex_t* semaforo_mutex) {
	pthread_mutex_lock(semaforo_mutex);
	list_add(lista, pcb);
	log_info(kernel_logger, "Proceso PID:%i ingreso en %s", pcb->pid, modulo);
	pthread_mutex_unlock(semaforo_mutex);
}

