/*
 * pcb.c
 *
 *  Created on: Apr 23, 2023
 *      Author: utnso
 */

#include "pcb.h"

t_pcb* crear_pcb(uint32_t pid, t_list* instrucciones, uint32_t estimacion){
	t_pcb* pcb = malloc(sizeof(t_pcb));
	pcb->pid = pid;
	pcb->instrucciones = instrucciones;
	pcb->program_counter = 0;
	pcb->estado = 0;
	init_registros(&pcb->registros_cpu);
	init_segmento(&pcb->tabla_segmentos);
	pcb->estimado_rafaga = estimacion;
	return pcb;
}


void init_registros(t_registros** registros){
	*registros = malloc(sizeof(t_registros));
	strcpy((*registros)->ax, "");
	strcpy((*registros)->bx, "");
	strcpy((*registros)->cx, "");
	strcpy((*registros)->dx, "");
	strcpy((*registros)->eax, "");
	strcpy((*registros)->ebx, "");
	strcpy((*registros)->ecx, "");
	strcpy((*registros)->edx, "");
	strcpy((*registros)->rax, "");
	strcpy((*registros)->rbx, "");
	strcpy((*registros)->rcx, "");
	strcpy((*registros)->rdx, "");
}

void init_segmento(t_list** tabla_segmentos){
	printf("inicializando segmento\n");
	(*tabla_segmentos)=list_create();
	printf("list_size: %d\n", list_size(*tabla_segmentos));
	/**segmento = malloc(sizeof(t_segmento));
	(*segmento)->id = 0;
	(*segmento)->direccio_base = 0;
	(*segmento)->tamanio_segmento = tamanio;*/
}

// ------------------------------------------------------------------------------------------
// -- Print en pantalla --
// ------------------------------------------------------------------------------------------

void print_pcb(t_pcb* pcb){
	printf("PCB: \n");
	printf("PID = %d\n", pcb->pid);
	printf("Program Counter = %u\n", pcb->program_counter);
	printf("Estado = %s\n", print_estado(pcb->estado));
	printf("Estimacion rafaga = %d\n", pcb->estimado_rafaga);
	//list_iterate(pcb->instrucciones, (void*)iterator);
	print_registos(pcb->registros_cpu);
	print_segmento(pcb->tabla_segmentos);
}

void print_contexto(t_contexto_ejecucion* contexto){
	printf("PCB: \n");
	printf("PID = %d\n", contexto->pid);
	printf("Program Counter = %u\n", contexto->program_counter);
	print_registos(contexto->registros_cpu);
	print_segmento(contexto->tabla_segmentos);
}


void print_registos(t_registros* registros){
	printf("Registros:\n");
	printf("AX = %s\n", registros->ax);
	printf("BX = %s\n", registros->bx);
	printf("CX = %s\n", registros->cx);
	printf("DX = %s\n", registros->dx);

	printf("EAX = %s\n", registros->eax);
	printf("EBX = %s\n", registros->ebx);
	printf("ECX = %s\n", registros->ecx);
	printf("EDX = %s\n", registros->edx);

	printf("RAX = %s\n", registros->rax);
	printf("RBX = %s\n", registros->rbx);
	printf("RCX = %s\n", registros->rcx);
	printf("RDX = %s\n", registros->rdx);
}

char* print_estado(estado_code estado){
	if(estado == NEW) 		return "New";
	if(estado == READY) 		return "Ready";
	if(estado == BLOCKED) 	return "Blocked";
	if(estado == EXEC) 	return "Exec";
	if(estado == EXIT) 	return "Exit";
	else			 			return "Error";
}

void print_segmento(t_list* tabla_segmentos){

	printf("cantidad segmentos: %d\n", list_size(tabla_segmentos));
	for(int i=0;i<list_size(tabla_segmentos);i++){
		t_segmento* segmento = list_get(tabla_segmentos, i);
		printf("Segmento:\n");
		printf("Id = %d\n", segmento->id);
		printf("Direccion = %d\n", segmento->direccio_base);
		printf("Tamanio = %d\n", segmento->tamanio_segmento);
	}

}


