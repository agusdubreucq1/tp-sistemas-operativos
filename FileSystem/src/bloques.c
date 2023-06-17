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
    	archivo_bloques = fopen(path_bloques, "w+");
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
