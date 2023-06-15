/*
 * fcb.c
 *
 *  Created on: Jun 13, 2023
 *      Author: utnso
 */

#include "fcb.h"

t_fcb* crear_fcb(char* nombre, uint32_t directo, uint32_t indirecto){
	t_fcb* fcb = malloc(sizeof(t_fcb));
	fcb->nombre_archivo = nombre;
	fcb->tamano_archivo = 0;
	fcb->puntero_directo = directo;
	fcb->puntero_indirecto = indirecto;
	return fcb;
}

void inicializar_fcbs(){
    DIR* directory = opendir(path_fcb);
    if (directory == NULL) {
        log_error(fileSystem_logger, "No se pudo abrir el directorio");
        return;
    }
    struct dirent* entry;
    while ((entry = readdir(directory)) != NULL) {
        if (entry->d_type == DT_REG) {
            const char* filename = entry->d_name;
            if (strstr(filename, ".dat") != NULL) {
                printf("Nombre de archivo: %s\n", filename);
                //list_add(element);
            }
        }
    }
    closedir(directory);
}

void leer_fcb(char* nombre){
	printf("AAAAAAAA");
	//printf("Ruta %s", path_fcb);
	//printf("Ruta %s", nombre);
	char* path_archivo = "";
	//printf("Ruta %s", path_archivo);
	strcat(path_archivo, path_fcb);
	printf("Ruta %s", path_archivo);

	//strcat(path_archivo, "/");
	//strcat(path_archivo, nombre);

	//printf("Ruta %s", path_archivo);

	/*t_config* config_fcb = config_create(path_fcb);

	tamanio_bloque = config_get_int_value(configSuperBloque, "BLOCK_SIZE");
	cant_bloques = config_get_int_value(configSuperBloque, "BLOCK_COUNT");
	log_trace(logger, "Super Bloque Montado");
	log_trace(logger, "Tamaño de Bloque: %d", tamanio_bloque);
	log_trace(logger, "Cantidad de Bloques: %d", cant_bloques);
	config_destroy(configSuperBloque);*/
}





