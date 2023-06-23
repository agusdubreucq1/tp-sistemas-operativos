/*
 * superbloque.h
 *
 *  Created on: Jun 13, 2023
 *      Author: utnso
 */

#ifndef SUPERBLOQUE_H_
#define SUPERBLOQUE_H_

#include <commons/log.h>
#include <commons/config.h>

extern int tamanio_bloque;
extern int cant_bloques;

// ------------------------------------------------------------------------------------------
// -- Funciones --
// ------------------------------------------------------------------------------------------


void levantar_superBloque(char* path, t_log* logger);
int redondearArriba(double x);
int bloques_necesarios(int bytes);


#endif /* SUPERBLOQUE_H_ */
