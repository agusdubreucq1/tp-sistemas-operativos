/*
 * parser.c
 *
 *  Created on: Apr 10, 2023
 *      Author: utnso
 */

#include "parser.h"

char* parsearInstrucciones(char* path){
	char c = '\0';
	char caracter[1];
	char* buffer = string_new();
	FILE* archivo;

	archivo = fopen(path, "r");

	while ((c = fgetc(archivo)) != EOF){
		caracter[0] = c;
		string_append(&buffer, caracter);
	}
	fclose(archivo);
	return buffer;
}
