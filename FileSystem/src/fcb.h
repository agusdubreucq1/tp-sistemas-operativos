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
#include "bitmap.h"
#include <commons/config.h>


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
extern t_list* lista_fcb;

// ------------------------------------------------------------------------------------------
// -- Funciones --
// ------------------------------------------------------------------------------------------



t_fcb* crear_fcb(char* nombre);
//void inicializar_fcbs();
void inicializar_FCBs();
void grabar_fcb(t_fcb* fcb);
t_fcb* fcb_segun_nombre(char* archivo);
t_fcb* leer_fcb(char* nombre, uint32_t tamanio, uint32_t puntero_directo, uint32_t puntero_indirecto);
//void leer_fcb(char* name);

#endif /* FCB_H_ */
