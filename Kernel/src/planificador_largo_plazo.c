/*
 * planificacion.c
 *
 *  Created on: Apr 25, 2023
 *      Author: utnso
 */

#include "planificador_largo_plazo.h"

void ingresar_en_lista(t_pcb* pcb, t_list* lista, char* modulo, pthread_mutex_t* semaforo_mutex, sem_t* semaforo, estado_code estado) {
	pthread_mutex_lock(semaforo_mutex);
	if (pcb->estado != estado){
		log_cambiar_estado(pcb->pid, pcb->estado, estado);
	}
	pcb->estado = estado;
	list_add(lista, pcb);
	sem_post(semaforo);

	//log_info(kernel_logger, "Proceso PID:%i ingreso en %s", pcb->pid, modulo);

	if(strcmp(modulo, "READY")==0){
		char* log_cola_ready = string_new();
		string_append(&log_cola_ready, "[");
		for(int i=0; i<list_size(lista); i++){
			t_pcb* pcb_logueado = list_get(lista, i);
			char* string_pid = string_itoa(pcb_logueado->pid);
			string_append(&log_cola_ready, string_pid);
			free(string_pid);
			if(i!= (list_size(lista)-1)){
				string_append(&log_cola_ready, ", ");
			}
		}
		string_append(&log_cola_ready, "]");
		log_info(kernel_logger,"Cola Ready %s : %s",algoritmo_planificacion, log_cola_ready);
		free(log_cola_ready);
	}
pthread_mutex_unlock(semaforo_mutex);
}

void log_cambiar_estado(int pid, estado_code viejo, estado_code nuevo){
	log_info(kernel_logger, "PID: %d - Cambio de estado %s -> %s", pid, codigo_estado_string(viejo), codigo_estado_string(nuevo));
}
