/*
 * bitmap.c
 *
 *  Created on: Jun 13, 2023
 *      Author: utnso
 */

#include "bitmap.h"

int inicializar_bitmap() {

    char* bloque = (char*) malloc(cant_bloques);

	bitmap = bitarray_create_with_mode(bloque ,cant_bloques, MSB_FIRST);
    if (bitmap == NULL) {
    	log_error(fileSystem_logger, "No se pudo crear el bitmap");
        return 1;
    }

    FILE* file = fopen(path_bitmap, "rb+");

    if (file == NULL) {
        file = fopen(path_bitmap, "wb+");
        if (file == NULL) {
        	log_error(fileSystem_logger, "No se pudo abrir el archivo bitmap.dat.");
        	bitarray_destroy(bitmap);
            return 1;
        }
        log_trace(fileSystem_logger, "Creando archivo bitmap.dat");
        fwrite(bitmap->bitarray, 1, cant_bloques / 8, file);

    }
    else {
    	log_trace(fileSystem_logger, "Creando Bitmap desde archivo -> bitmap.dat");
        fread(bitmap->bitarray, 1, (cant_bloques) / 8, file);
    }

    //imprimir_bitmap(bitmap);

    fclose(file);
    free(bloque);
    log_trace(fileSystem_logger, "Bitmap inicializado");
    return 0;
}

void imprimir_bitmap(t_bitarray* bitmap){

	for (int i = 0; i < bitmap->size; i++) {
    	//bitarray_clean_bit(bitmap, i);
        printf("%d ", bitarray_test_bit(bitmap, i));
    }
}

