/*
 * recursos.c
 *
 *  Created on: May 25, 2023
 *      Author: utnso
 */

#include "recursos.h"

int recurso_existe(char* nombre){
    int recurso_existe = -1;
    for (int i = 0; i < list_size(lista_recursos); i++) {
    	t_recurso* recurso = list_get(lista_recursos, i);
        if (string_equals_ignore_case(nombre, recurso->nombre)) {
        	recurso_existe = true;
        	return i;
        }
    }
    return recurso_existe;
}

void descontar_recurso(t_recurso* recurso, t_pcb* pcb, t_log* logger){
	recurso->cantidad -= 1;
	list_add(pcb->recursos, recurso);
	log_info(logger, "PID: %d - Wait: %s - Instancias: %d", pcb->pid, recurso->nombre, recurso->cantidad);
	if (recurso->cantidad < 0){
		estimar_rafaga(pcb);
		list_add(recurso->listaBloqueados, pcb);
		log_cambiar_estado(pcb->pid, pcb->estado, BLOCKED);
		log_info(logger, "PID: %d - Bloqueado por: %s", pcb->pid, recurso->nombre);
		pcb->estado = BLOCKED;
	} else {
		//ingresar_en_lista(pcb, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);//vuelve a ejecutar el mismo proceso
		devolver_ejecucion = 1;
		//pcb_ejecutando = pcb;
	}
}

void sumar_recurso(t_recurso* recurso, t_pcb* pcb, t_log* logger){
	recurso->cantidad += 1;
	list_remove_element(pcb->recursos, recurso);
	log_info(logger, "PID: %d - Signal: %s - Instancias: %d", pcb->pid, recurso->nombre, recurso->cantidad);
	if (list_size(recurso->listaBloqueados) > 0){
		t_pcb* pcb_bloqueado = list_remove(recurso->listaBloqueados, 0);
		log_info(logger, "PID: %d - Desbloqueado por: %s", pcb_bloqueado->pid, recurso->nombre);
		mandar_a_ready(pcb_bloqueado);
	}
}
