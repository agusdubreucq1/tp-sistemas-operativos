/*
 * bloques.h
 *
 *  Created on: Jun 14, 2023
 *      Author: utnso
 */

#ifndef BLOQUES_H_
#define BLOQUES_H_

#include <stdio.h>
#include <commons/log.h>

extern t_log* fileSystem_logger;
extern int tamanio_bloque;
extern int cant_bloques;
extern char* path_bloques;

void inicializar_bloques();

#endif /* BLOQUES_H_ */
