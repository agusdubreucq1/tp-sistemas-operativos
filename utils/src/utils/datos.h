/*
 * estructuras.h
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#ifndef SRC_DATOS_H_
#define SRC_DATOS_H_

#include <commons/collections/list.h>
#include <commons/log.h>
#include <commons/config.h>
#include <stdint.h>

// ------------------------------------------------------------------------------------------
// -- Structs --
// ------------------------------------------------------------------------------------------


typedef enum
{
	MENSAJE,
	PAQUETE,
	PCB,
	INSTRUCCION
} op_code;

typedef struct{
	int size;
	void* stream;
} t_buffer;

typedef struct{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

typedef struct{
	char *ip;
	char *puerto;
	t_log* logger;
	char *modulo;
}kernelThreadParams;

// ------------------------------------------------------------------------------------------
// -- CPU --
// ------------------------------------------------------------------------------------------

typedef struct registros_cpu{
	char ax[4], bx[4], cx[4], dx[4];
	char eax[8], ebx[8], ecx[8], edx[8];
	char rax[16], rbx[16], rcx[16], rdx[16];
}t_registros;


// ------------------------------------------------------------------------------------------
// -- MEMORIA --
// ------------------------------------------------------------------------------------------

typedef struct segmento{
	uint32_t id;
	uint32_t direccio_base;
	uint32_t tamanio_segmento;
} t_segmento;


// ------------------------------------------------------------------------------------------
// -- Kernel --
// ------------------------------------------------------------------------------------------

typedef enum {
    NEW,
    READY,
    EXEC,
    BLOCKED,
	EXITT,
	ERROR
} estado_code;

typedef struct pcb {
	uint32_t pid;
	t_list* instrucciones;
	uint32_t program_counter;
	t_registros* registros_cpu;
	t_list* tabla_segmentos;
	uint32_t estimado_rafaga;
	uint32_t tiempo_ready;
	//uint32_t tabla_archivos;
	estado_code estado;
}t_pcb;

typedef struct contexto_ejecucion {
	uint32_t pid;
	t_list* instrucciones;
	uint32_t program_counter;
	t_registros* registros_cpu;
	t_list* tabla_segmentos;
}t_contexto_ejecucion;

typedef struct retorno_contexto {
	uint32_t pid;
	uint32_t program_counter;
	t_registros* registros_cpu;
}t_retorno_contexto;



#endif /* SRC_DATOS_H_ */
