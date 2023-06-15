/*
 * serializar.h
 *
 *  Created on: May 5, 2023
 *      Author: utnso
 */

#ifndef UTILS_SERIALIZAR_H_
#define UTILS_SERIALIZAR_H_

#include "datos.h"
#include "utils.h"
#include "pcb.h"
#include "cliente.h"
#include <commons/collections/list.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// ------------------------------------------------------------------------------------------
// -- Funciones --
// ------------------------------------------------------------------------------------------

t_paquete* serializar_pcb(t_pcb* pcb);
t_paquete* serializar_contexto(t_contexto_ejecucion* contexto);
void agregar_variable_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void serializar_registros_cpu(t_paquete* paquete, t_registros* registro);
void serializar_instrucciones(t_paquete* paquete, t_pcb* pcb);
void serializar_tabla_segmentos(t_paquete* paquete, t_pcb* pcb);
t_paquete* serializar_segmentos(t_tabla_segmentos* tabla);

#endif /* UTILS_SERIALIZAR_H_ */
