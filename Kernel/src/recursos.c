/*
 * recursos.c
 *
 *  Created on: May 25, 2023
 *      Author: utnso
 */

#include "recursos.h"

int recurso_existe(char nombre[20]){
    int recurso_existe = -1;

    for (int i = 0; i < list_size(lista_recursos); i++) {
    	t_recurso* recurso = list_get(lista_recursos, i);
        if (strcmp(recurso->nombre, nombre) == 0) {
        	recurso_existe = true;
        	return i;
        }
    }
    return recurso_existe;
}

void descontar_recurso(t_recurso* recurso, t_pcb* pcb){
	recurso->cantidad -= 1;
	if (recurso->cantidad < 0){
		list_add(recurso->listaBloqueados, pcb);
		pcb->estado = BLOCKED;
		printf("\n\n\n\n\n\nBLOQUEADOOOOOOOOOOOO\n\n\n\n\n");
	} else {
		ingresar_en_lista(pcb, lista_ready, "READY", &semaforo_ready, READY);
		sem_post(&cantidad_procesos_ready);
	}
}

void sumar_recurso(t_recurso* recurso){
	recurso->cantidad += 1;
	if (list_size(recurso->listaBloqueados) > 0){
		t_pcb* pcb_bloqueado = list_remove(recurso->listaBloqueados, 0);
		ingresar_en_lista(pcb_bloqueado, lista_ready, "READY", &semaforo_ready, READY);
		printf("\n\n\n\n\n\nDESBLOQUEADOOOOOOOOOOOO\n\n\n\n\n");
		sem_post(&cantidad_procesos_ready);
	}
}
