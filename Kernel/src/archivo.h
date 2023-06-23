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
#include <commons/string.h>


// ------------------------------------------------------------------------------------------
// -- Estructuras --
// ------------------------------------------------------------------------------------------

typedef struct archivo{
	char* nombre;

	uint32_t puntero;
	t_list* listaBloqueados;
} t_archivo;

// ------------------------------------------------------------------------------------------
// -- variables --
// ------------------------------------------------------------------------------------------

extern t_list* lista_archivos_abiertos;

// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------


t_archivo* crear_archivo(char* nombre);
t_archivo* buscar_archivo_abierto(char* nombre_archivo);
void agregar_archivo_al_proceso(t_archivo* archivo, t_pcb* pcb);
//void cerrar_archivo(char* nombre_archivo, t_pcb* pcb);
t_archivo* get_archivo_del_proceso(char* nombre_archivo, t_pcb* pcb);
void listar_tabla_del_proceso(t_pcb* pcb);
void listar_tabla_de_archivos_pcb(t_pcb* pcb);

#endif /* ARCHIVO_H_ */
