/*
 * instruccion.c
 *
 *  Created on: May 12, 2023
 *      Author: utnso
 */

#include "instruccion.h"

t_instruccion* parsear_instruccion(char* linea){
	char** parametros = string_split(linea, " ");
	t_instruccion* nueva_instruccion = malloc(sizeof(t_instruccion));
	nueva_instruccion->codigo_instruccion = obtener_codigo_instruccion(parametros[0]);

	if(contarElementos(parametros) == 1 ){
		nueva_instruccion->parametro[0] = "                ";
		nueva_instruccion->parametro[1] = "                ";
		nueva_instruccion->parametro[2] = "                ";
	} else if(contarElementos(parametros) == 2 ){
		nueva_instruccion->parametro[0] = string_duplicate(parametros[1]);
		nueva_instruccion->parametro[1] = "                ";
		nueva_instruccion->parametro[2] = "                ";
	} else if(contarElementos(parametros) == 3 ){
		nueva_instruccion->parametro[0] = string_duplicate(parametros[1]);
		nueva_instruccion->parametro[1] = string_duplicate(parametros[2]);
		nueva_instruccion->parametro[2] = "                ";
	} else{
		nueva_instruccion->parametro[0] = string_duplicate(parametros[1]);
		nueva_instruccion->parametro[1] = string_duplicate(parametros[2]);
		nueva_instruccion->parametro[2] = string_duplicate(parametros[3]);
	}
	string_array_destroy(parametros);
	return nueva_instruccion;
}

codigo_instruccion obtener_codigo_instruccion(char* codigo){
	if(string_equals_ignore_case(codigo, "SET")) 			return SET;
	if(string_equals_ignore_case(codigo, "MOV_IN")) 		return MOV_IN;
	if(string_equals_ignore_case(codigo, "MOV_OUT")) 		return MOV_OUT;
	if(string_equals_ignore_case(codigo, "I/O")) 			return I_O;
	if(string_equals_ignore_case(codigo, "I_O")) 			return I_O;
	if(string_equals_ignore_case(codigo, "F_OPEN")) 		return F_OPEN;
	if(string_equals_ignore_case(codigo, "F_CLOSE")) 		return F_CLOSE;
	if(string_equals_ignore_case(codigo, "F_SEEK")) 		return F_SEEK;
	if(string_equals_ignore_case(codigo, "F_READ")) 		return F_READ;
	if(string_equals_ignore_case(codigo, "F_WRITE")) 		return F_WRITE;
	if(string_equals_ignore_case(codigo, "F_TRUNCATE"))		return F_TRUNCATE;
	if(string_equals_ignore_case(codigo, "WAIT")) 			return WAIT;
	if(string_equals_ignore_case(codigo, "SIGNAL")) 		return SIGNAL;
	if(string_equals_ignore_case(codigo, "CREATE_SEGMENT"))	return CREATE_SEGMENT;
	if(string_equals_ignore_case(codigo, "DELETE_SEGMENT"))	return DELETE_SEGMENT;
	if(string_equals_ignore_case(codigo, "YIELD"))			return YIELD;
	if(string_equals_ignore_case(codigo, "INICIAR"))		return INICIAR;
	if(string_equals_ignore_case(codigo, "SEGMENT"))		return SEGMENT;
	if(string_equals_ignore_case(codigo, "OUT"))			return OUT;
	if(string_equals_ignore_case(codigo, "COMPACT"))		return COMPACT;
	if(string_equals_ignore_case(codigo, "FINALIZAR"))		return FINALIZAR;
	if(string_equals_ignore_case(codigo, "F_CREATE"))		return F_CREATE;
	else										 			return EXIT;
}

char* codigo_instruccion_string(codigo_instruccion codigo){
	if(codigo == SET) 				return "SET";
	if(codigo == MOV_IN) 			return "MOV_IN";
	if(codigo == MOV_OUT) 			return "MOV_OUT";
	if(codigo == I_O) 				return "I/O";
	if(codigo == F_OPEN) 			return "F_OPEN";
	if(codigo == F_CLOSE) 			return "F_CLOSE";
	if(codigo == F_SEEK) 			return "F_SEEK";
	if(codigo == F_READ) 			return "F_READ";
	if(codigo == F_WRITE) 			return "F_WRITE";
	if(codigo == F_TRUNCATE)	 	return "F_TRUNCATE";
	if(codigo == WAIT) 				return "WAIT";
	if(codigo == SIGNAL) 			return "SIGNAL";
	if(codigo == CREATE_SEGMENT)	return "CREATE_SEGMENT";
	if(codigo == DELETE_SEGMENT) 	return "DELETE_SEGMENT";
	if(codigo == YIELD) 			return "YIELD";
	if(codigo == INICIAR) 			return "INICIAR";
	if(codigo == SEGMENT) 			return "SEGMENT";
	if(codigo == OUT) 				return "OUT";
	if(codigo == COMPACT) 			return "COMPACT";
	if(codigo == FINALIZAR) 		return "FINALIZAR";
	else							return "EXIT";
}

int contarElementos(char** arreglo) {
    int contador = 0;

    while (arreglo[contador] != NULL) {
        contador++;
    }
    return contador;
}
