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

	contexto_deserializado->tabla_segmentos = deserializar_tabla_segmentos(stream, &desplazamiento);
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

		strncpy(registros->ax, deserializar_char(stream, desplazamiento, 4), 4);
		strncpy(registros->bx, deserializar_char(stream, desplazamiento, 4), 4);
		strncpy(registros->cx, deserializar_char(stream, desplazamiento, 4), 4);
		strncpy(registros->dx, deserializar_char(stream, desplazamiento, 4), 4);
		strncpy(registros->eax, deserializar_char(stream, desplazamiento, 8), 8); /*-> usar strncpy, sino se copiara hasta encontrar un \0*/
		strncpy(registros->ebx, deserializar_char(stream, desplazamiento, 8), 8);
		strncpy(registros->ecx, deserializar_char(stream, desplazamiento, 8), 8);
		strncpy(registros->edx, deserializar_char(stream, desplazamiento, 8), 8);
		strncpy(registros->rax, deserializar_char(stream, desplazamiento, 16), 16);
		strncpy(registros->rbx, deserializar_char(stream, desplazamiento, 16), 16);
		strncpy(registros->rcx, deserializar_char(stream, desplazamiento, 16), 16);
		strncpy(registros->rdx, deserializar_char(stream, desplazamiento, 16), 16);


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
	 char* registro;

	 registro = stream + *desplazamiento;
	 *desplazamiento+=tamanio;
	 return registro;

 }

uint32_t deserializar_uint32(void* stream, int* desplazamiento){
	uint32_t* variable;

	variable = stream + *desplazamiento;
	*desplazamiento+= sizeof(uint32_t);

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
	printf("\n\n\nVAAAA \n\n\n");
	int pid = deserializar_uint32(stream, desplazamiento);
	tabla->pid = pid;
	printf("\n\n\nPIDDD %d\n\n\n", pid);
	int cant_segmentos = deserializar_uint32(stream, desplazamiento);

	//int tamanio = 0;

	for(int j=0; j < cant_segmentos; j++){
		t_segmento* segmento = malloc(sizeof(t_segmento));
		//memcpy(&tamanio, stream + *desplazamiento, sizeof(int));
		//*desplazamiento += sizeof(int);
		memcpy(&(segmento->direccion_base), stream + *desplazamiento, sizeof(void*));
		*desplazamiento += sizeof(void*);
		memcpy(&(segmento->limite), stream + *desplazamiento, sizeof(void*));
		*desplazamiento += sizeof(void*);
		list_add(tabla->segmentos, &segmento);
	}
	return tabla;
}

