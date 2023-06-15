/*
 * tablas_segmento.h
 *
 *  Created on: Jun 15, 2023
 *      Author: utnso
 */

#ifndef TABLAS_SEGMENTO_H_
#define TABLAS_SEGMENTO_H_

#include <utils/datos.h>
#include <stdint.h>
#include <commons/collections/list.h>
#include <stdlib.h>
#include <utils/serializar.h>

// ------------------------------------------------------------------------------------------
// -- Variables --
// ------------------------------------------------------------------------------------------

extern 	t_log* memoria_logger;
extern char* cant_segmentos;
extern t_segmento* segmento_cero;

// ------------------------------------------------------------------------------------------
// -- Funciones --
// ------------------------------------------------------------------------------------------

t_tabla_segmentos* crear_tabla(uint32_t pid);
t_segmento* crear_segmento(void* base, void* tamanio);
void enviar_segmentos(t_tabla_segmentos* tabla, int socket);

#endif /* TABLAS_SEGMENTO_H_ */
