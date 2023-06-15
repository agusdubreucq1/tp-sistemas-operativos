/*
 * archivo.c
 *
 *  Created on: Jun 14, 2023
 *      Author: utnso
 */

#include "archivo.h"


t_archivo* crear_archivo(char* nombre, int fcb){
	t_archivo* archivo = malloc(sizeof(t_archivo));
	archivo->nombre = nombre;
	archivo->fcb = fcb;
	archivo->listaBloqueados = list_create();
	return archivo;
}

t_archivo* buscar_archivo_abierto(char* nombre_archivo){
	for(int i=0; i< list_size(lista_archivos_abiertos); i++){
		t_archivo* archivo_comparar = list_get(lista_archivos_abiertos, i);
		printf("Archivo Comp %s\n", archivo_comparar->nombre);
		printf("Archivo Comp %s\n", nombre_archivo);
		if(strcmp(archivo_comparar->nombre, nombre_archivo) == 0){
			return archivo_comparar;
		}
	}
	return NULL;
}

