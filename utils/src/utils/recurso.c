/*
 * recurso.c
 *
 *  Created on: May 25, 2023
 *      Author: utnso
 */

#include "recurso.h"

t_recurso* crear_recurso(char* nombre, int cantidad){
	t_recurso* recurso = malloc(sizeof(t_recurso));
	recurso->nombre = string_new();
	string_append(&recurso->nombre,nombre);
	recurso->cantidad = cantidad;
	recurso->listaBloqueados = list_create();

	return recurso;
}

void imprimir_recurso(t_recurso* recurso){
	//printf("Recurso: %s", recurso->nombre);
	//printf("Cantidad: %i", recurso->cantidad);
	//printf("Procesos Bloqueados: \n");
}
