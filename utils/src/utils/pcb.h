/*
 * pcb.h
 *
 *  Created on: Apr 23, 2023
 *      Author: utnso
 */

#ifndef PCB_H_
#define PCB_H_

#include "datos.h"
#include "utils.h"
#include <commons/collections/list.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// ------------------------------------------------------------------------------------------
// -- Funciones --
// ------------------------------------------------------------------------------------------

t_pcb* crear_pcb(uint32_t pid, t_list* instrucciones, uint32_t estimacion, uint32_t tamanio);
void init_registros(t_registros** registros);
void init_segmento(t_segmento** segmento, uint32_t tamanio);
void print_pcb(t_pcb* pcb);
void print_registos(t_registros* registros);
void print_lista(char* value);
char* print_estado(estado_code estado);
void print_segmento(t_segmento* segmento);




#endif /* PCB_H_ */
