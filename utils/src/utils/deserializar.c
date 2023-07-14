/*
 * deserializar.c
 *
 *  Created on: May 5, 2023
 *      Author: utnso
 */
#include "deserializar.h"


t_contexto_ejecucion* deserializar_pcb(void* stream,int* bytes_recibidos){
	t_contexto_ejecucion* contexto_deserializado= malloc(sizeof(t_contexto_ejecucion));
	int desplazamiento = 0;

	contexto_deserializado->pid = deserializar_uint32(stream, &desplazamiento);

	contexto_deserializado->instrucciones = deserializar_instrucciones(stream, &desplazamiento);

	contexto_deserializado->program_counter = deserializar_uint32(stream, &desplazamiento);

	contexto_deserializado->registros_cpu =  deserializar_registros_cpu(stream, &desplazamiento);
	//print_registos(contexto_deserializado->registros_cpu);

	contexto_deserializado->tabla_segmentos = deserializar_segmentos(stream, &desplazamiento);
	//print_segmento(contexto_deserializado->tabla_segmentos);

	*bytes_recibidos = desplazamiento;

	return contexto_deserializado;
}

void deserializar_contexto(void* stream,int* bytes_recibidos, t_pcb* pcb){
	int desplazamiento = 0;

	pcb->program_counter = deserializar_uint32(stream, &desplazamiento);
	pcb->registros_cpu =  deserializar_registros_cpu(stream, &desplazamiento);

	*bytes_recibidos = desplazamiento;
}

char* deserializar_motivo(void* stream, int* desplazamiento){
	int tam_char = deserializar_uint32(stream, desplazamiento);
	char* motivo = deserializar_char(stream, desplazamiento, tam_char);
	return motivo;

}

t_registros* deserializar_registros_cpu(void* stream, int* desplazamiento){
	t_registros* registros=malloc(sizeof(t_registros));

	char* str_ax = deserializar_char(stream, desplazamiento, 4);
	char* str_bx = deserializar_char(stream, desplazamiento, 4);
	char* str_cx = deserializar_char(stream, desplazamiento, 4);
	char* str_dx = deserializar_char(stream, desplazamiento, 4);
	char* str_eax = deserializar_char(stream, desplazamiento, 8);
	char* str_ebx=deserializar_char(stream, desplazamiento, 8);
	char* str_ecx = deserializar_char(stream, desplazamiento, 8);
	char * str_edx = deserializar_char(stream, desplazamiento, 8);
	char* str_rax =deserializar_char(stream, desplazamiento, 16);
	char* str_rbx =deserializar_char(stream, desplazamiento, 16);
	char* str_rcx =deserializar_char(stream, desplazamiento, 16);
	char* str_rdx = deserializar_char(stream, desplazamiento, 16);


	strncpy(registros->ax,str_ax , 4);
	strncpy(registros->bx,str_bx, 4);
	strncpy(registros->cx,str_cx , 4);
	strncpy(registros->dx, str_dx, 4);
	strncpy(registros->eax, str_eax, 8); /*-> usar strncpy, sino se copiara hasta encontrar un \0*/
	strncpy(registros->ebx, str_ebx, 8);
	strncpy(registros->ecx,str_ecx , 8);
	strncpy(registros->edx,str_edx , 8);
	strncpy(registros->rax,str_rax , 16);
	strncpy(registros->rbx, str_rbx, 16);
	strncpy(registros->rcx,str_rcx, 16);
	strncpy(registros->rdx,str_rdx, 16);

	free(str_ax);
	free(str_bx);
	free(str_cx);
	free(str_dx);
	free(str_eax);
	free(str_ebx);
	free(str_ecx);
	free(str_edx);
	free(str_rax);
	free(str_rbx);
	free(str_rcx);
	free(str_rdx);

	return registros;
}

t_list* deserializar_tabla_segmentos(void* stream, int* desplazamiento){
	t_list* tabla_segmentos= list_create();
	int cant_segmentos = deserializar_uint32(stream, desplazamiento);
	int tamanio;

	for(int j=0; j<cant_segmentos; j++){
		t_segmento segmento;
		memcpy(&tamanio, stream + *desplazamiento, sizeof(int));
		*desplazamiento += sizeof(int);
		memcpy(&segmento, stream + *desplazamiento, sizeof(t_segmento));
		*desplazamiento += sizeof(t_segmento);
		list_add(tabla_segmentos, &segmento);
	}
	return tabla_segmentos;
}

 char* deserializar_char(void* stream, int* desplazamiento, int tamanio){
	 char* registro= malloc(tamanio);

	 memcpy(registro, stream + *desplazamiento, tamanio);
	 //registro = stream + *desplazamiento;
	 *desplazamiento+=tamanio;
	 return registro;
 }

uint32_t deserializar_uint32(void* stream, int* desplazamiento){
	uint32_t* variable;

	variable = stream + *desplazamiento;
	*desplazamiento+= sizeof(uint32_t);

	return *variable;
}

bool deserializar_bool(void* stream, int* desplazamiento){
	bool* variable;

	variable = stream + *desplazamiento;
	*desplazamiento+= sizeof(bool);

	return *variable;
}

t_list* deserializar_instrucciones(void* stream, int* desplazamiento){
	int cant_instrucciones = deserializar_uint32(stream, desplazamiento);
	int tamanio;
	t_list* lista_instrucciones= list_create();
	for(int i=0; i<cant_instrucciones; i++){
			char* instruccion;
			memcpy(&tamanio, stream + *desplazamiento, sizeof(int));
			*desplazamiento+=sizeof(int);
			instruccion = malloc(tamanio);
			memcpy(instruccion, stream + *desplazamiento, tamanio);
			*desplazamiento+=tamanio;
			instruccion[tamanio-1]='\0';
			//printf("instruccion: %s \n", instruccion);

			list_add(lista_instrucciones, instruccion);
		}
	return lista_instrucciones;
}

t_tabla_segmentos* deserializar_segmentos(void* stream, int* desplazamiento){
	t_tabla_segmentos* tabla = malloc(sizeof(t_tabla_segmentos));
	tabla->segmentos = list_create();
	int pid = deserializar_uint32(stream, desplazamiento);
	tabla->pid = pid;
	int cant_segmentos = deserializar_uint32(stream, desplazamiento);
	for(int j=0; j < cant_segmentos; j++){
		t_segmento* segmento = malloc(sizeof(t_segmento));
		memcpy(&(segmento->direccion_base), stream + *desplazamiento, sizeof(void*));
		*desplazamiento += sizeof(void*);
		memcpy(&(segmento->limite), stream + *desplazamiento, sizeof(void*));
		*desplazamiento += sizeof(void*);
		segmento->pid = pid;
		segmento->id = j;
		if(segmento->direccion_base != NULL){
			segmento->libre = 0;
		} else{
			segmento->libre = 1;
		}

		list_add(tabla->segmentos, segmento);

	}
	return tabla;
}
