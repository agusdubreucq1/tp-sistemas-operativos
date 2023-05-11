#include "paquete_a_enviar.h"

// ------------------------------------------------------------------------------------------
// -- Funciones parapaquetes y buffers en general --
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------- Paquetes

void* serializar_paquete(t_paquete* paquete){

	int tamanio_a_enviar = sizeof(paquete->codigo_operacion) + sizeof(paquete->buffer->size) + paquete->buffer->size;

	//Inicia puntero vacio
	void* total_paquete = malloc(tamanio_a_enviar);
	int desplazamiento = 0;

	//Copia codigo de operacion (la primer parte del mensaje que va a llegar)
	memcpy(total_paquete + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento += sizeof(int);

	//Copia tamanio del paquete
	memcpy(total_paquete + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento += sizeof(int);

	//Copia contenido del paquete
	memcpy(total_paquete + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento += paquete->buffer->size;

	return total_paquete;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio){

	//'Agranda' el paquete para agregar un valor
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	//Copia el tamanio del valor
	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));

	//Copia el valor
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	//Actualiza el tamanio del paquete (se agrego dato y tamanio del dato)
	paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete* paquete, int socket_cliente){

	//Total mensaje = buffer + tamanio buffer + codigo operacion
	int bytes = paquete->buffer->size + 2 * sizeof(int);


	//Transforma de t_paquete a *void
	void* a_enviar = serializar_paquete(paquete);

	//Envia datos
	if(send(socket_cliente, a_enviar, bytes, 0) <= 0)
		perror("Los datos no se enviaron correctamente");


	//Liberar datos
	free(a_enviar);
}

t_paquete* crear_paquete(t_buffer* buffer, op_code codigo_de_operacion){
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = codigo_de_operacion;
	paquete->buffer = buffer;
	return paquete;
}

void eliminar_paquete(t_paquete* paquete){
	//Primer eliminar *void del paquete
	free(paquete->buffer->stream);

	//Despues el puntero que guardaba la estructura t_buffer
	free(paquete->buffer);

	//Despues el puntero que guardaba a t_paquete
	free(paquete);
}

op_code recibir_operacion(int socket_cliente){
	int codigo_operacion;
	if(recv(socket_cliente, &codigo_operacion, sizeof(u_int32_t), MSG_WAITALL) >= 0){
		return codigo_operacion;
	}else{
		printf("Hubo un error al recibir la operacion del cliente... Cerrando conexión \n");
		close(socket_cliente);
		return -1;
	}
}

t_buffer* recibir_buffer(int socket_cliente) {
	t_buffer *buffer = buffer_create();
	recv(socket_cliente, &(buffer->size), sizeof(buffer->size), MSG_WAITALL);
	buffer->stream = malloc(buffer->size);
	recv(socket_cliente, buffer->stream, buffer->size, MSG_WAITALL);
	return buffer;
}

// ------------------------------------------------------------------------------------------
// -- Funciones para streams --
// ------------------------------------------------------------------------------------------


int agregar_char_en_stream(int offset, t_buffer *buffer, char caracter) {
	//Agregar espacio entre codigo_instruccion y primer_parametro
	buffer->stream = realloc(buffer->stream, offset + 1);
	memcpy(buffer->stream + offset, &caracter, 1);
	return offset + 1;
}


int agregar_eof_en_stream(int offset, t_buffer *buffer) {
	//Agregar espacio entre codigo_instruccion y primer_parametro
	return agregar_char_en_stream(offset, buffer, '\0');
}


int agregar_new_line_en_stream(int offset, t_buffer *buffer) {
	//Agregar espacio entre codigo_instruccion y primer_parametro
	return agregar_char_en_stream(offset, buffer, '\n');
}


int agregar_espacio_en_stream(int offset, t_buffer *buffer) {
	//Agregar espacio entre codigo_instruccion y primer_parametro
	return agregar_char_en_stream(offset, buffer, ' ');
}
