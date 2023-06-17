/*
 * bitmap.c
 *
 *  Created on: Jun 16, 2023
 *      Author: utnso
 */

#include "bitmap_memoria.h"

void inicializar_bitmap(){

    char* bloque = (char*) malloc(atoi(tam_memoria));

	bitmap = bitarray_create_with_mode(bloque ,atoi(tam_memoria), MSB_FIRST);
	//imprimir_bitmap(bitmap);
	free(bloque);
}

void imprimir_bitmap(t_bitarray* bitmap){

	for (int i = 0; i < bitmap->size; i++) {
    	//bitarray_clean_bit(bitmap, i);
        printf("%d", bitarray_test_bit(bitmap, i));
    }
}

int validar_huecos_libres(t_bitarray* bitmap, int inicio, int cant) {
	int huecos_validos = 1;
	for (int i=inicio; i<inicio+cant; i++) {
		if (bitarray_test_bit(bitmap, i) == 1) {
			huecos_validos = 0;
		}
	}

	return huecos_validos;
}

void ocupar_bitmap(t_bitarray* bitmap, int inicio, int cant) {
	if (validar_huecos_libres(bitmap, inicio, cant)) {
		for (int i=inicio; i<inicio+cant; i++) {
				bitarray_set_bit(bitmap, i);
			}
	}
	else {
		printf("\nLos huecos donde se desea escribir no estan libres");
	}

}

void liberar_bitmap(t_bitarray* bitmap, int inicio, int cant) {
	for (int i=inicio; i<inicio+cant; i++) {
		bitarray_clean_bit(bitmap, i);
		}
}

int first_fit_bitmap(t_bitarray* bitmap, int tamanio) {
	int cont_huecos_libres = 0;
	for (int i = 0; i < bitmap->size; i++) {
		if (cont_huecos_libres == tamanio) {
			return i-tamanio;
			//Devuelve la primera posicion tal que bitmap[posicion] esta libre y tiene *tamanio* huecos libres contiguos
		}
		else {
			if (bitarray_test_bit(bitmap, i) == 0) {
				cont_huecos_libres++;
			}
			else {
				cont_huecos_libres = 0;
			}
		}
	}
	return -1;
}
