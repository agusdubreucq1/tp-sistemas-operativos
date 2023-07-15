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
	pcb->tabla_segmentos = malloc(sizeof(t_tabla_segmentos));
	pcb->tabla_segmentos->segmentos = list_create();
	//init_segmento(&pcb->tabla_segmentos);
	pcb->estimado_rafaga = estimacion;
	pcb->llegada_cpu = 0;
	pcb->tabla_archivos = list_create();
	pcb->recursos = list_create();
	return pcb;
}


void init_registros(t_registros** registros){
	*registros = malloc(sizeof(t_registros));
	strncpy((*registros)->ax, "    ",4);
	strncpy((*registros)->bx, "    ",4);
	strncpy((*registros)->cx, "    ",4);
	strncpy((*registros)->dx, "    ",4);
	strncpy((*registros)->eax, "        ",8);
	strncpy((*registros)->ebx, "        ",8);
	strncpy((*registros)->ecx, "        ",8);
	strncpy((*registros)->edx, "        ",8);
	strncpy((*registros)->rax, "                ",16);
	strncpy((*registros)->rbx, "                ",16);
	strncpy((*registros)->rcx, "                ",16);
	strncpy((*registros)->rdx, "                ",16);
}

void init_segmento(t_list** tabla_segmentos){
	//printf("inicializando segmento\n");
	(*tabla_segmentos)=list_create();
	//printf("list_size: %d\n", list_size(*tabla_segmentos));
	/**segmento = malloc(sizeof(t_segmento));
	(*segmento)->id = 0;
	(*segmento)->direccio_base = 0;
	(*segmento)->tamanio_segmento = tamanio;*/
}

// ------------------------------------------------------------------------------------------
// -- Print en pantalla --
// ------------------------------------------------------------------------------------------

void print_pcb(t_pcb* pcb){/*
	printf("PCB: \n");
	printf("PID = %d\n", pcb->pid);
	printf("Program Counter = %u\n", pcb->program_counter);
	printf("Estado = %s\n", print_estado(pcb->estado));
	printf("Tiempo en ready= %u\n", pcb->tiempo_ready);
	printf("Estimacion rafaga = %d\n", pcb->estimado_rafaga);*/
	//list_iterate(pcb->instrucciones, (void*)iterator);
	print_registos(pcb->registros_cpu);
	//print_segmento(pcb->tabla_segmentos);
}

void print_contexto(t_contexto_ejecucion* contexto){/*
	printf("PCB: \n");
	printf("PID = %d\n", contexto->pid);
	printf("Program Counter = %u\n", contexto->program_counter);*/
	print_registos(contexto->registros_cpu);
	//print_segmento(contexto->tabla_segmentos);
}


void print_registos(t_registros* registros){
/*
	printf("Registros:\n");
	print_registro("AX",4, registros->ax);
	print_registro("BX",4, registros->bx);
	print_registro("CX",4, registros->cx);
	print_registro("DX",4, registros->dx);

	print_registro("EAX",8, registros->eax);
	print_registro("EBX",8, registros->ebx);
	print_registro("ECX",8, registros->ecx);
	print_registro("EDX",8, registros->edx);

	print_registro("RAX",16, registros->rax);
	print_registro("RBX",16, registros->rbx);
	print_registro("RCX",16, registros->rcx);
	print_registro("RDX",16, registros->rdx);
	printf("\n\n");*/
}

void print_registro(char* nombre, int tamanio, char* registro){
	//printf("\n%s = ", nombre);
		for(int i=0; i<tamanio;i++){
			//printf("%c",registro[i]);
		}
}

char* print_estado(estado_code estado){
	if(estado == NEW) 		return "New";
	if(estado == READY) 	return "Ready";
	if(estado == BLOCKED) 	return "Blocked";
	if(estado == EXEC) 		return "Exec";
	if(estado == EXITT) 		return "Exit";
	else			 		return "Error";
}

void print_segmento(t_list* tabla_segmentos){

	//printf("\ncantidad segmentos: %d\n", list_size(tabla_segmentos));
	for(int i=0;i<list_size(tabla_segmentos);i++){
		t_segmento* segmento = list_get(tabla_segmentos, i);
		//printf("Segmento:\n");
		//printf("Id = %p\n", segmento->id);
		//printf("Direccion = %p\n", segmento->direccion_base);
		//printf("Tamanio = %p\n", segmento->limite);
	}

}


