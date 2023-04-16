/*
 * estructuras.h
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#ifndef SRC_DATOS_H_
#define SRC_DATOS_H_

// ------------------------------------------------------------------------------------------
// -- Structs --
// ------------------------------------------------------------------------------------------


typedef enum
{
	MENSAJE,
	PAQUETE
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;



#endif /* SRC_DATOS_H_ */
