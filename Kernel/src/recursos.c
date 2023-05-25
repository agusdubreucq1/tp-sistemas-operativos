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
	} else {
		ingresar_en_lista(pcb, lista_ready, "READY", &semaforo_ready, READY);
		sem_post(&cantidad_procesos_ready);
	}
}
