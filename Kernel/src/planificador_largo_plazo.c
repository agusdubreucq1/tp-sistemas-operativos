/*
 * planificacion.c
 *
 *  Created on: Apr 25, 2023
 *      Author: utnso
 */

#include "planificador_largo_plazo.h"

void ingresar_en_lista(t_pcb* pcb, t_list* lista, char* modulo, pthread_mutex_t* semaforo_mutex, estado_code estado) {
	pthread_mutex_lock(semaforo_mutex);
	if (pcb->estado != estado){
		log_cambiar_estado(pcb->pid, pcb->estado, estado);
	}
	pcb->estado = estado;
	list_add(lista, pcb);
	//log_info(kernel_logger, "Proceso PID:%i ingreso en %s", pcb->pid, modulo);
	pthread_mutex_unlock(semaforo_mutex);
}

void log_cambiar_estado(int pid, estado_code viejo, estado_code nuevo){
	log_info(kernel_logger, "PID: %d - Cambio de estado %s -> %s", pid, codigo_estado_string(viejo), codigo_estado_string(nuevo));
}
