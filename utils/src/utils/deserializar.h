/*
 * deserializar.h
 *
 *  Created on: May 5, 2023
 *      Author: utnso
 */

#ifndef UTILS_DESERIALIZAR_H_
#define UTILS_DESERIALIZAR_H_

#include "datos.h"
#include "utils.h"
#include <commons/collections/list.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcb.h"

// ------------------------------------------------------------------------------------------
// -- Funciones --
// ------------------------------------------------------------------------------------------

t_contexto_ejecucion* deserializar_pcb(void* stream,int* bytes_recibidos);
void deserializar_contexto(void* stream,int* bytes_recibidos, t_pcb* pcb);
char* deserializar_motivo(void* stream, int* desplazamiento);
t_registros* deserializar_registros_cpu(void* stream, int* desplazamiento);
t_list* deserializar_tabla_segmentos(void* stream, int* desplazamiento);
 char* deserializar_char(void* stream, int* desplazamiento, int tamanio);
uint32_t deserializar_uint32(void* stream, int* desplazamiento);
t_list* deserializar_instrucciones(void* stream, int* desplazamiento);
t_tabla_segmentos* deserializar_segmentos(void* stream, int* desplazamiento);
bool deserializar_bool(void* stream, int* desplazamiento);





#endif /* UTILS_DESERIALIZAR_H_ */
