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

void agregar_archivo_tabla_del_proceso(t_archivo* archivo, t_pcb* pcb){
	printf("\nel archivo a agregar archivo->nombre: %s\n\n", archivo->nombre);
	t_archivo_de_proceso* archivo_proceso = malloc(sizeof(t_archivo_de_proceso));
	archivo_proceso->archivo = malloc(sizeof(t_archivo));
	memcpy(archivo_proceso->archivo, archivo, sizeof(t_archivo));
	archivo_proceso->puntero = 0;
	list_add(pcb->tabla_archivos,archivo_proceso);
}

t_archivo_de_proceso* get_archivo_del_proceso(char* nombre_archivo, t_pcb* pcb){
	//t_archivo_de_proceso* archivo;
	for(int i=0; i< list_size(pcb->tabla_archivos);i++){
		t_archivo_de_proceso* archivo_de_proceso = list_get(pcb->tabla_archivos, i);
		t_archivo* archivo = archivo_de_proceso->archivo;
		char* nombre_a_comparar = archivo->nombre;
		printf("\ncomparando: %s con %s\n\n",nombre_archivo,nombre_a_comparar);
		if(string_equals_ignore_case(nombre_archivo,nombre_a_comparar)){
			return list_get(pcb->tabla_archivos, i);
		}
	}
	return NULL;
}

void listar_tabla_del_proceso(t_pcb* pcb){
	if(list_size(pcb->tabla_archivos)==0){
		printf("\nla tabla del proceso esta vacia\n\n");
	}else{
		printf("\nla tabla del proceso contiene: ");
		for(int i=0; i< list_size(pcb->tabla_archivos);i++){
			t_archivo_de_proceso* archivo_de_proceso = list_get(pcb->tabla_archivos,0);
			t_archivo* archivo = archivo_de_proceso->archivo;
			printf(" %s   ", archivo->nombre);
		}
		printf("\n\n");
	}
}


