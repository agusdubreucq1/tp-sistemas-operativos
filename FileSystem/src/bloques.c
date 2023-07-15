/*
 * bloques.c
 *
 *  Created on: Jun 14, 2023
 *      Author: utnso
 */

#include "bloques.h"

void inicializar_bloques(){

    FILE* archivo_bloques = fopen(path_bloques, "r+");
    int tamanio_archivo = cant_bloques * tamanio_bloque;

    if (archivo_bloques == NULL) {
        // El archivo no existe, se crea y se trunca al tamaño deseado

    	archivo_bloques = fopen(path_bloques, "wb+");
        if (archivo_bloques == NULL) {
            log_trace(fileSystem_logger, "No se pudo crear Archivo de Bloques");
            return;
        }
        fseek(archivo_bloques, tamanio_archivo - 1, SEEK_SET);
        fputc('\0', archivo_bloques);

        log_trace(fileSystem_logger, "Archivo de Bloques creado");
        log_trace(fileSystem_logger, "Tamaño: %d", tamanio_archivo);
        //fclose(archivo_bloques);
    } else {

    	log_trace(fileSystem_logger, "Archivo de Bloques leido");
    	log_trace(fileSystem_logger, "Tamaño: %d", tamanio_archivo);
        /*// El archivo existe, se verifica el tamaño actual
        fseek(archivo_bloques, tamanio_archivo - 1, SEEK_END);
        if (ftell(archivo_bloques) == tamanio_archivo) {
        	log_trace(fileSystem_logger, "Archivo de Bloques leido");
        	log_trace(fileSystem_logger, "Tamaño: %d", tamanio_archivo);
        } else {
        	log_error(fileSystem_logger, "Archivo de Bloques no tiene el tamaño deseado.");
        	return;
        }*/
    }
    fclose(archivo_bloques);
}

void leerBloque(int bloque){
	FILE* file = fopen(path_bloques, "rb");
	fseek(file, bloque*64,SEEK_SET);
	//printf("\n\nimprimiendo bloque %d\n", bloque);
	uint32_t puntero;
	for(int i=0;i<16;i++){
		fread(&puntero,4,1,file);
		//printf("%d ", puntero);
	}
	//printf("\n\n");
	fclose(file);
}

void leerArchivoBloques(void* aLeer, int posicion, int cantidad){
	FILE* archivo_bloques = fopen(path_bloques, "r+b");
	fseek(archivo_bloques, posicion, SEEK_SET);
	fread(aLeer, cantidad,1, archivo_bloques);
	fclose(archivo_bloques);
}

void escribirArchivoBloques(void* aEscribir, int posicion, int cantidad){
	log_trace(fileSystem_logger, "escribiendo archivo pos: %d , cant: %d", posicion, cantidad);
	FILE* archivo_bloques = fopen(path_bloques, "r+b");
	fseek(archivo_bloques, posicion, SEEK_SET);
	fwrite(aEscribir, cantidad,1, archivo_bloques);
	fclose(archivo_bloques);
}
