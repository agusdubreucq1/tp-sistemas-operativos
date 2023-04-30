#ifndef PAQUETE_A_ENVIAR_H_
#define PAQUETE_A_ENVIAR_H_

#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <commons/log.h>
#include <commons/collections/list.h>

#include "buffer.h"
#include "instrucciones.h"

typedef enum {
	MENSAJE,
	NUEVO_PROCESO,				//Envío de un t_paq_instrucciones_a_kernel
	EJECUTAR_PCB,				//Envío normal de un pcb
	ACTUALIZAR_PCB,				//Envío de un pcb para actualizar los valores de otro
	TERMINO_PCB,				//Envío de un pcb listo para terminar (Pasa a estado EXIT)
	APAGAR_SISTEMA,				//Lo envía el Kernel para finalizar los procesos Memoria y CPU
	DESALOJO_PCB_POR_QUANTUM,	//Lo pone la CPU para mandar el pcb a kernel y ponerlo en ready (RR o Feedback)
	IO_PANTALLA,
	IO_TECLADO,
	IO_DISPOSITIVO,
	INTERRUPCION,
	CREAR_TABLA_SEGMENTO,		//Lo envía el Kernel a Memoria para obtener el índice de una tabla de segemntos
	PAGE_FAULT,
	INICIO_MEMORIA,				//Lo envía Memoria a MMU para que sepa el tamanio_max_segmento
	CONSULTAR_FRAME,
	SEGMENTATION_FAULT,
	MOVIN,
	MOVOUT
}op_code;


typedef struct {
	op_code codigo_operacion; 	//header
	t_buffer* buffer; 			//payload + payload_size
} t_paquete;


// ------------------------------------------------------------------------------------------
// -- Funciones para paquetes y buffers en general --
// ------------------------------------------------------------------------------------------

op_code recibir_operacion(int socket_cliente);

// ------------------------------------------------------------------- Paquete
void eliminar_paquete(t_paquete* paquete);
void* serializar_paquete(t_paquete* paquete);
t_paquete* crear_paquete(t_buffer* buffer, op_code codigo_de_operacion);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
t_buffer* recibir_buffer(int socket_cliente);
// ------------------------------------------------------------------------------------------
// -- Funciones para streams --
// ------------------------------------------------------------------------------------------

int agregar_eof_en_stream(int offset, t_buffer *buffer);
int agregar_new_line_en_stream(int offset, t_buffer *buffer);
int agregar_espacio_en_stream(int offset, t_buffer *buffer);
int agregar_char_en_stream(int offset, t_buffer *buffer, char caracter);

#endif /* PAQUETE_A_ENVIAR_H_ */
