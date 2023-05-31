#include "registros_cpu.h"


void registros_put(t_registros* registros, char* registro, char* nuevo_valor){
	if(string_equals_ignore_case(registro, "ax")) 	strncpy(registros->ax, nuevo_valor, 4);
	if(string_equals_ignore_case(registro, "bx")) 	strncpy(registros->bx, nuevo_valor, 4);
	if(string_equals_ignore_case(registro, "cx")) 	strncpy(registros->cx, nuevo_valor, 4);
	if(string_equals_ignore_case(registro, "dx")) 	strncpy(registros->dx, nuevo_valor, 4);
	if(string_equals_ignore_case(registro, "eax")) 	strncpy(registros->eax, nuevo_valor, 8);
	if(string_equals_ignore_case(registro, "ebx")) 	strncpy(registros->ebx, nuevo_valor, 8);
	if(string_equals_ignore_case(registro, "ecx")) 	strncpy(registros->ecx, nuevo_valor, 8);
	if(string_equals_ignore_case(registro, "edx")) 	strncpy(registros->edx, nuevo_valor, 8);
	if(string_equals_ignore_case(registro, "rax")) 	strncpy(registros->rax, nuevo_valor, 16);
	if(string_equals_ignore_case(registro, "rbx")) 	strncpy(registros->rbx, nuevo_valor, 16);
	if(string_equals_ignore_case(registro, "rcx")) 	strncpy(registros->rcx, nuevo_valor, 16);
	if(string_equals_ignore_case(registro, "rdx")) 	strncpy(registros->rdx, nuevo_valor, 16);
}

void iniciar_registros(t_registros* registros){
	strcpy(registros->ax, "    ");
	strcpy(registros->bx, "    ");
	strcpy(registros->cx, "    ");
	strcpy(registros->dx, "    ");
	strcpy(registros->eax, "        ");
	strcpy(registros->ebx, "        ");
	strcpy(registros->ecx, "        ");
	strcpy(registros->edx, "        ");
	strcpy(registros->rax, "                ");
	strcpy(registros->rbx, "                ");
	strcpy(registros->rcx, "rcxrcxrcxrcxrcxr");
	strcpy(registros->rdx, "                ");
}


void imprimir_registros(t_registros* registros){
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

