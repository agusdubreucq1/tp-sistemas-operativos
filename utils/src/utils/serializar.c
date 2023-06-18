/*
 * serializar.c
 *
 *  Created on: May 5, 2023
 *      Author: utnso
 */
#include "serializar.h"

t_paquete* serializar_pcb(t_pcb* pcb){
	t_paquete* paquete = crear_paquete();

	agregar_variable_a_paquete(paquete, &(pcb->pid), sizeof(int));
	serializar_instrucciones(paquete, pcb);
	agregar_variable_a_paquete(paquete, &(pcb->program_counter), sizeof(uint32_t));
	serializar_registros_cpu(paquete, pcb->registros_cpu);
	serializar_tabla_segmentos(paquete, pcb);
	//printf("tam_paquete: %ld\n", paquete->buffer->size + 2*sizeof(int));

	return paquete;
}

t_paquete* serializar_contexto(t_contexto_ejecucion* contexto){
	t_paquete* paquete = crear_paquete();

	//agregar_variable_a_paquete(paquete, &(contexto->pid), sizeof(uint32_t));
	agregar_variable_a_paquete(paquete, &(contexto->program_counter), sizeof(uint32_t));
	serializar_registros_cpu(paquete, contexto->registros_cpu);
	//printf("tam_paquete: %ld\n", paquete->buffer->size + 2*sizeof(int));

	return paquete;
}

void agregar_variable_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio);

	memcpy(paquete->buffer->stream + paquete->buffer->size, valor, tamanio);

	paquete->buffer->size += tamanio;
	//printf("\n buffer->size: %d\n", paquete->buffer->size);
}


void serializar_registros_cpu(t_paquete* paquete, t_registros* registro){
	agregar_variable_a_paquete(paquete, registro->ax, 4);
	agregar_variable_a_paquete(paquete, registro->bx, 4);
	agregar_variable_a_paquete(paquete, registro->cx, 4);
	agregar_variable_a_paquete(paquete, registro->dx, 4);
	agregar_variable_a_paquete(paquete, registro->eax, 8);
	agregar_variable_a_paquete(paquete, registro->ebx, 8);
	agregar_variable_a_paquete(paquete, registro->ecx, 8);
	agregar_variable_a_paquete(paquete, registro->edx, 8);
	agregar_variable_a_paquete(paquete, registro->rax, 16);
	agregar_variable_a_paquete(paquete, registro->rbx, 16);
	agregar_variable_a_paquete(paquete, registro->rcx, 16);
	agregar_variable_a_paquete(paquete, registro->rdx, 16);
	//print_registos(registro);
}

void serializar_instrucciones(t_paquete* paquete, t_pcb* pcb){
	int cant_instrucciones = list_size(pcb->instrucciones);
	agregar_variable_a_paquete(paquete, &cant_instrucciones, sizeof(int));
	for(int i=0;list_size(pcb->instrucciones)>i;i++){
		//printf("\n instruccion %d: %s -> tam: %ld \n", i, (char*)list_get(pcb->instrucciones, i), strlen(list_get(pcb->instrucciones, i)));
		agregar_a_paquete(paquete, list_get(pcb->instrucciones, i), strlen(list_get(pcb->instrucciones, i))+1);
	}
}

void serializar_tabla_segmentos(t_paquete* paquete, t_pcb* pcb){
	agregar_variable_a_paquete(paquete, &(pcb->pid), sizeof(int));
	int cant_segmentos = list_size(pcb->tabla_segmentos->segmentos);
	agregar_variable_a_paquete(paquete, &cant_segmentos, sizeof(int));
	for(int i = 0; i < cant_segmentos; i++){
		t_segmento* segmento = list_get(pcb->tabla_segmentos->segmentos, i);
		agregar_variable_a_paquete(paquete, &(segmento->direccion_base), sizeof(void*));
		agregar_variable_a_paquete(paquete, &(segmento->limite), sizeof(void*));
	}
}

t_paquete* serializar_segmentos(t_tabla_segmentos* tabla){
	t_paquete* paquete = crear_paquete();
	agregar_variable_a_paquete(paquete, &(tabla->pid), sizeof(int));
	int cant_segmentos = list_size(tabla->segmentos);
	agregar_variable_a_paquete(paquete, &cant_segmentos, sizeof(int));
	for(int i = 0; i < list_size(tabla->segmentos); i++){
		t_segmento* segmento = list_get(tabla->segmentos, i);
		agregar_variable_a_paquete(paquete, &(segmento->direccion_base), sizeof(void*));
		agregar_variable_a_paquete(paquete, &(segmento->limite), sizeof(void*));
	}
	//printf("tam_paquete: %ld\n", paquete->buffer->size + 2*sizeof(int));
	return paquete;
}
