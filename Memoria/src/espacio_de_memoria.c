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

void imprimir_memoria() {

	printf("\nEntre a imprimir_memoria!");
	for (int i=0; i<atoi(tam_memoria); i++) {
		if (*(char *)(memoria_fisica+i) != '\0') {
			printf("\nValor en %p: %c", (memoria_fisica+i), *(char *)(memoria_fisica+i));
		}
	}
}

/*
void escribir_mensaje_en_memoria(void *direccion_fisica, char *valor, int tamanio) {
	printf("\nEntre a escribir_mensaje_en_memoria!");
	int posicion_bitmap = devolver_posicion_bitmap_segun_direccion(direccion_fisica);
	if (validar_huecos_libres(posicion_bitmap, tamanio)) {
		printf("\nHay huecos libres para ocupar");
		//Aca deberiamos meter un semaforo
		memcpy(direccion_fisica, valor, tamanio);
		ocupar_bitmap(posicion_bitmap, tamanio);
		imprimir_memoria();
		imprimir_bitmap();
	}
	else {
		printf("\nNo se puede escribir en la memoria porque está ocupada");
	}

}
*/
