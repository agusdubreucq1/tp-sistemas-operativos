#include "espacio_de_memoria.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* reservar_espacio_memoria(){

	return malloc(atoi(tam_memoria));

	/*
	 * En Memoria.c, al momento de hacer el print de sizeof(memoria_fisica) no me está devolviendo
	 * la cantidad de Bytes que acá le estoy asignando con el malloc.
	 * Valor esperado: 4096 (archivo config), Valor printeado: 8 Bytes
	 * Curiosamente, sizeof(void*) = 8, y memoria_fisica es un void* (dejo el print en Memoria.c)
	 */

}

void imprimir_memoria_segun_base_y_tam(void *base, int tamanio) {

	//printf("\nEntre a imprimir_memoria!");
	for (int i=0; i<tamanio; i++) {
		//printf("\nValor en %p: %c", (base+i), *(char *)(base+i));
	}
}

void imprimir_memoria() {

	//printf("\nEntre a imprimir_memoria!");
	for (int i=0; i<atoi(tam_memoria); i++) {
		if (*(char *)(memoria_fisica+i) != '\0') {
			//printf("\nPosicion bitmap: %d; Contenido en %p: %c", i,(memoria_fisica+i), *(char *)(memoria_fisica+i));
		}
	}
}

