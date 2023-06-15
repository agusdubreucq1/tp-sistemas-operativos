/*
 * archivo.h
 *
 *  Created on: Jun 14, 2023
 *      Author: utnso
 */

#ifndef ARCHIVO_H_
#define ARCHIVO_H_

#include <stdlib.h>
#include <commons/collections/list.h>
#include <string.h>
#include <stdio.h>

// ------------------------------------------------------------------------------------------
// -- Estructuras --
// ------------------------------------------------------------------------------------------

typedef struct archivo{
	char* nombre;
	int fcb;
	t_list* listaBloqueados;
} t_archivo;

// ------------------------------------------------------------------------------------------
// -- variables --
// ------------------------------------------------------------------------------------------

extern t_list* lista_archivos_abiertos;

// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

t_archivo* crear_archivo(char* nombre, int fcb);
t_archivo* buscar_archivo_abierto(char* nombre_archivo);

#endif /* ARCHIVO_H_ */
