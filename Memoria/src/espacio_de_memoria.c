#include "espacio_de_memoria.h"
#include <stdio.h>
#include <stdlib.h>

void* reservar_espacio_memoria(){

	return malloc(atoi(tam_memoria));

	/*
	 * En Memoria.c, al momento de hacer el print de sizeof(memoria_fisica) no me está devolviendo
	 * la cantidad de Bytes que acá le estoy asignando con el malloc.
	 * Valor esperado: 4096 (archivo config), Valor printeado: 8 Bytes
	 * Curiosamente, sizeof(void*) = 8, y memoria_fisica es un void* (dejo el print en Memoria.c)
	 */

}
