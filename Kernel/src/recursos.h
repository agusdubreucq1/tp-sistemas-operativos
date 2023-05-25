/*
 * recursos.h
 *
 *  Created on: May 25, 2023
 *      Author: utnso
 */

#ifndef RECURSOS_H_
#define RECURSOS_H_

#include <commons/collections/list.h>
#include <string.h>
#include <utils/recurso.h>
#include <semaphore.h>
#include "planificador_largo_plazo.h"

// ------------------------------------------------------------------------------------------
// -- Estructuras Planificacion --
// ------------------------------------------------------------------------------------------

	extern t_list* lista_ready;
	extern t_list* lista_recursos;

	extern pthread_mutex_t semaforo_ready;
	extern sem_t cantidad_procesos_ready;

// ------------------------------------------------------------------------------------------
// -- Funciones --
// ------------------------------------------------------------------------------------------

	int recurso_existe(char nombre[20]);
	void descontar_recurso(t_recurso* recurso, t_pcb* pcb);
	void sumar_recurso(t_recurso* recurso);

#endif /* RECURSOS_H_ */