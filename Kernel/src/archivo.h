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
#include <utils/datos.h>

// ------------------------------------------------------------------------------------------
// -- Estructuras --
// ------------------------------------------------------------------------------------------

typedef struct archivo{
	char* nombre;
	int fcb;
	t_list* listaBloqueados;
} t_archivo;

typedef struct archivo_de_proceso{
	t_archivo* archivo;
	uint32_t puntero;
} t_archivo_de_proceso;

// ------------------------------------------------------------------------------------------
// -- variables --
// ------------------------------------------------------------------------------------------

extern t_list* lista_archivos_abiertos;

// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

t_archivo* crear_archivo(char* nombre, int fcb);
t_archivo* buscar_archivo_abierto(char* nombre_archivo);
void agregar_archivo_tabla_del_proceso(t_archivo* archivo, t_pcb* pcb);

#endif /* ARCHIVO_H_ */
