/*
 * fcb.h
 *
 *  Created on: Jun 13, 2023
 *      Author: utnso
 */

#ifndef FCB_H_
#define FCB_H_

#include <stdint.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <commons/string.h>
#include <stdlib.h>

// ------------------------------------------------------------------------------------------
// -- Estructuras--
// ------------------------------------------------------------------------------------------

typedef struct fcb {
	char* nombre_archivo;
	uint32_t tamano_archivo;
	uint32_t puntero_directo;
	uint32_t puntero_indirecto;
} t_fcb;

extern char* path_fcb;
extern t_log* fileSystem_logger;

// ------------------------------------------------------------------------------------------
// -- Funciones --
// ------------------------------------------------------------------------------------------


t_fcb* crear_fcb(char* nombre);
void inicializar_fcbs();
//void leer_fcb(char* name);

#endif /* FCB_H_ */
