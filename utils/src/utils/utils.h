/*
 * utils.h
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_


#include <stdio.h>
#include <stdlib.h>

#include <commons/log.h>
#include <commons/config.h>
#include "datos.h"

// ------------------------------------------------------------------------------------------
// -- Logger --
// ------------------------------------------------------------------------------------------

	t_log* iniciar_logger(char* path, char* nombre_modulo);

// ------------------------------------------------------------------------------------------
// -- Config --
// ------------------------------------------------------------------------------------------

	t_config* iniciar_config(char* config, char* nombre_modulo);

// ------------------------------------------------------------------------------------------
// -- Funciones --
// ------------------------------------------------------------------------------------------

	void iterator(char* value);
	char* codigo_estado_string(estado_code codigo);
	void imprimir_segmentos(t_tabla_segmentos* tabla);

#endif /* SRC_UTILS_H_ */
