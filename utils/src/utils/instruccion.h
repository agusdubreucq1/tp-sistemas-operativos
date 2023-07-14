/*
 * instruccion.h
 *
 *  Created on: May 12, 2023
 *      Author: utnso
 */

#ifndef UTILS_INSTRUCCION_H_
#define UTILS_INSTRUCCION_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <commons/string.h>

// ------------------------------------------------------------------------------------------
// -- CPU --
// ------------------------------------------------------------------------------------------

typedef enum {
	 SET,
	 MOV_IN,
	 MOV_OUT,
	 I_O,
	 F_OPEN,
	 F_CLOSE,
	 F_SEEK,
	 F_READ,
	 F_WRITE,
	 F_TRUNCATE,
	 WAIT,
	 SIGNAL,
	 CREATE_SEGMENT,
	 DELETE_SEGMENT,
	 YIELD,
	 EXIT,
	 INICIAR,
	 SEGMENT,
	 OUT,
	 COMPACT,
	 FINALIZAR,
	 F_CREATE
}codigo_instruccion;

typedef struct {
	uint32_t codigo_instruccion;
    char* parametro[3];
}t_instruccion;

// ------------------------------------------------------------------------------------------
// -- FUNCIONES --
// ------------------------------------------------------------------------------------------

t_instruccion* parsear_instruccion(char* linea);
codigo_instruccion obtener_codigo_instruccion(char* codigo);
int contarElementos(char** arreglo);
char* codigo_instruccion_string(codigo_instruccion codigo);

#endif /* UTILS_INSTRUCCION_H_ */
