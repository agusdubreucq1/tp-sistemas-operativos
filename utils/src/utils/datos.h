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
	void* direccion_base;
	void* limite;
	uint32_t id;
	uint32_t pid;
	bool libre;
} t_segmento;


typedef struct tabla_de_segmento{
	uint32_t pid;
	t_list* segmentos;
} t_tabla_segmentos;

typedef enum
{
	FIRST,
	BEST,
	WORST
} alg_asignacion;

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
	t_tabla_segmentos* tabla_segmentos;
	uint32_t estimado_rafaga;
	uint32_t tiempo_ready;
	uint32_t llegada_cpu;
	t_list* tabla_archivos;
	t_list* recursos;
	estado_code estado;
}t_pcb;


typedef struct contexto_ejecucion {
	uint32_t pid;
	t_list* instrucciones;
	uint32_t program_counter;
	t_registros* registros_cpu;
	t_tabla_segmentos* tabla_segmentos;
}t_contexto_ejecucion;


typedef struct recurso{
	int cantidad;
	char* nombre;
	t_list* listaBloqueados;
} t_recurso;

typedef struct args_io{
   t_pcb* pcb;
   int duracion;
} t_thread_args;

typedef struct args_fileSystem{
	t_pcb* pcb;
	char* motivo;
}t_args_fileSystem;


#endif /* SRC_DATOS_H_ */
