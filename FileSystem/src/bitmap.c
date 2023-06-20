/*
 * bitmap.c
 *
 *  Created on: Jun 13, 2023
 *      Author: utnso
 */

#include "bitmap.h"

int inicializar_bitmap() {
	int size = cant_bloques / 8;
	char* bloque = malloc(size);
	memset(bloque, 0, size);
	bitmap = bitarray_create_with_mode(bloque, size, MSB_FIRST);
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
        printf("archivo bitmap no existia\n");

    }
    else {
    	log_trace(fileSystem_logger, "Creando Bitmap desde archivo -> bitmap.dat");
        fread(bitmap->bitarray, 1, (cant_bloques) / 8, file);
        printf("archivo bitmap ya existia\n");
    }

    imprimir_bitmap_20(bitmap);

    fclose(file);
    //free(bloque);
    log_trace(fileSystem_logger, "Bitmap inicializado");
    return 0;
}

int bloque_libre(){
	for(int i=0;i<bitmap->size*8; i++){
		if(bitarray_test_bit(bitmap, i)==0){
			return i;
		}
	}
	return -1;
}

uint32_t asignar_bloque(){
	uint32_t bloque_a_asignar;
	bloque_a_asignar = bloque_libre();
	bitarray_set_bit(bitmap, bloque_a_asignar);

	grabar_bitmap(bitmap);
	return bloque_a_asignar;
}

void grabar_bitmap(t_bitarray* bitmap){
	FILE* file = fopen(path_bitmap, "wb+");
	char* data = bitmap->bitarray;
	fwrite(data, 1, bitmap->size, file);
	fclose(file);//vuelve a escribir todo el archivo con el nuevo valor de bitmap actual
}

void imprimir_bitmap(t_bitarray* bitmap){
	printf("\nimprimiendo bitmap: \n[");
	//printf("bitmap->size: %ld\n", bitmap->size);
	for (int i = 0; i < bitmap->size*8; i++) {
    	//bitarray_clean_bit(bitmap, i);
        printf("%d ", bitarray_test_bit(bitmap, i));
    }
	printf("\n\n");
}

void imprimir_bitmap_20(t_bitarray* bitmap){
	printf("\n\nimprimiendo bitmap: \n[");
	//printf("bitmap->size: %ld\n", bitmap->size);
		for (int i = 0; i < 20; i++) {
	    	//bitarray_clean_bit(bitmap, i);
	        printf("%d ", bitarray_test_bit(bitmap, i));
	    }
		printf("]\n\n");
}

