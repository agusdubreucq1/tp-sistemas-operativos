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
