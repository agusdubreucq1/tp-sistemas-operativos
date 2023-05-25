/*
 * recurso.h
 *
 *  Created on: May 25, 2023
 *      Author: utnso
 */

#ifndef UTILS_RECURSO_H_
#define UTILS_RECURSO_H_

#include <stdlib.h>
#include "datos.h"
#include <commons/collections/list.h>
#include <string.h>

// ------------------------------------------------------------------------------------------
// -- Funciones --
// ------------------------------------------------------------------------------------------

	t_recurso* crear_recurso(char nombre[20], int cantidad);
	void imprimir_recurso(t_recurso* recurso);

#endif /* UTILS_RECURSO_H_ */
