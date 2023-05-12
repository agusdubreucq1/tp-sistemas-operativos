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

/*

void iniciar_registros(t_registros** registros){
	*registros = malloc(sizeof(t_registros));
	resetear_registros(*registros);
}

t_registro_unitario* crear_registro_unitario(){
	t_registro_unitario* registro = malloc(sizeof(t_registro_unitario));
	registro->registro = NULL;
	registro->valor = 0;
	return registro;
}

void resetear_registros(t_registros* registros){
	registros->ax = 0;
	registros->bx = 0;
	registros->cx = 0;
	registros->dx = 0;
	registros->eax = 0;
	registros->ebx = 0;
	registros->ecx = 0;
	registros->edx = 0;
	registros->rax = 0;
	registros->rbx = 0;
	registros->rcx = 0;
	registros->rdx = 0;
}


void imprimir_registros_por_pantalla(t_registros* registros){
	printf("-------------------\n");
	printf("Valores de los Registros:\n");
	printf(" + AX = %u\n", registros->ax);
	printf(" + BX = %u\n", registros->bx);
	printf(" + CX = %u\n", registros->cx);
	printf(" + DX = %u\n", registros->dx);
	printf(" + EAX = %u\n", registros->eax);
	printf(" + EBX = %u\n", registros->ebx);
	printf(" + ECX = %u\n", registros->ecx);
	printf(" + EDX = %u\n", registros->edx);
	printf(" + RAX = %u\n", registros->rax);
	printf(" + RBX = %u\n", registros->rbx);
	printf(" + RCX = %u\n", registros->rcx);
	printf(" + RDX = %u\n", registros->rdx);

	printf("-------------------\n");

}

void liberar_registros(t_registros* registos){
	free(registos);
}

void actualizar_registros_cpu(t_registros* registros_cpu, t_registros* nuevos_valores){
	registros_cpu->ax = nuevos_valores->ax;
	registros_cpu->bx = nuevos_valores->bx;
	registros_cpu->cx = nuevos_valores->cx;
	registros_cpu->dx = nuevos_valores->dx;
}

void registros_put(t_registros* registros, char* registro, uint32_t nuevo_valor){
	if(string_equals_ignore_case(registro, "ax")) 	registros->ax = nuevo_valor;
	if(string_equals_ignore_case(registro, "bx")) 	registros->bx = nuevo_valor;
	if(string_equals_ignore_case(registro, "cx")) 	registros->cx = nuevo_valor;
	if(string_equals_ignore_case(registro, "dx")) 	registros->dx = nuevo_valor;
	if(string_equals_ignore_case(registro, "eax")) 	registros->eax = nuevo_valor;
	if(string_equals_ignore_case(registro, "ebx")) 	registros->ebx = nuevo_valor;
	if(string_equals_ignore_case(registro, "ecx")) 	registros->ecx = nuevo_valor;
	if(string_equals_ignore_case(registro, "edx")) 	registros->edx = nuevo_valor;
	if(string_equals_ignore_case(registro, "rax")) 	registros->rax = nuevo_valor;
	if(string_equals_ignore_case(registro, "rbx")) 	registros->rbx = nuevo_valor;
	if(string_equals_ignore_case(registro, "rcx")) 	registros->rcx = nuevo_valor;
	if(string_equals_ignore_case(registro, "rdx")) 	registros->rdx = nuevo_valor;

}

uint32_t registros_get(t_registros* registros, char* registro){
	if(string_equals_ignore_case(registro, "ax")) 	return registros->ax;
	if(string_equals_ignore_case(registro, "bx")) 	return registros->bx;
	if(string_equals_ignore_case(registro, "cx")) 	return registros->cx;
	else											return registros->dx;
}


// ------------------------------------------------------------------------------------------
// -- Serialización y Deserialización --
// ------------------------------------------------------------------------------------------


t_buffer* serializar_registro_unitario_cpu(int offset, t_buffer *buffer, char* registro, u_int32_t valor) {

	buffer->stream = realloc(buffer->stream, offset + sizeof(u_int32_t));
	memcpy(buffer->stream + offset, registro, strlen("xx")+1);
	offset += strlen("xx")+1;

	buffer->stream = realloc(buffer->stream, offset + sizeof(u_int32_t));
	memcpy(buffer->stream + offset, &valor, sizeof(u_int32_t));
	offset += sizeof(u_int32_t);

	buffer->size = offset;
	return buffer;
}


void enviar_registro_unitario(char* registro, u_int32_t valor, op_code operacion, t_log* logger, int socket){

	t_buffer* buffer_registro_unitario = buffer_create();

	//Solo lo usamos para la impresión en pantalla
	buffer_registro_unitario = serializar_registro_unitario_cpu(0, buffer_registro_unitario, registro, valor);

	t_paquete* paquete_a_enviar = crear_paquete(buffer_registro_unitario, operacion);

	enviar_paquete(paquete_a_enviar, socket);

	eliminar_paquete(paquete_a_enviar);
}


t_registro_unitario* deserializar_registro_unitario(void *stream) {
	t_registro_unitario* registro_unitario = crear_registro_unitario();
	int offset = 0;

	registro_unitario->registro = malloc(strlen("xx") + 1);
	memcpy(registro_unitario->registro, stream + offset, strlen("XX") +1);
	offset += strlen("XX") + 1;

	memcpy(&registro_unitario->valor, stream + offset, sizeof(u_int32_t));
	offset += sizeof(u_int32_t);

	return registro_unitario;
}

t_registro_unitario* recibir_registro_unitario(int socket, t_log* logger){

	t_buffer* buffer = buffer_create();

	recv(socket, &(buffer->size), sizeof(buffer->size), MSG_WAITALL);

	buffer->stream = malloc(buffer->size);

	recv(socket, buffer->stream, buffer->size, MSG_WAITALL);

	t_registro_unitario* registro_unitario = deserializar_registro_unitario(buffer->stream);

	eliminar_buffer(buffer);
//	log_debug(logger, "El ID del PCB recibido es: %d", pcb->id);

	return registro_unitario;

}

void imprimir_registro_unitario_por_pantalla(t_registro_unitario* registro){
	printf("\n---------------------------------------------------------\n");
	printf("---------------------------------------------------------\n");
	printf("El valor del registro %s enviado por la CPU es = %u", registro->registro, registro->valor);
	printf("\n---------------------------------------------------------\n");
	printf("---------------------------------------------------------\n\n");
}

void destruir_registro_unitario(t_registro_unitario* registro){
	free(registro->registro);
	free(registro);
}
*/
