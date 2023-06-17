/*
 * fcb.c
 *
 *  Created on: Jun 13, 2023
 *      Author: utnso
 */

#include "fcb.h"

t_fcb* crear_fcb(char* nombre){
	t_fcb* fcb_nuevo= malloc(sizeof(t_fcb));
	fcb_nuevo->nombre_archivo = string_new();
	string_append(&fcb_nuevo->nombre_archivo, nombre);
	fcb_nuevo->tamano_archivo = 0;
	fcb_nuevo->puntero_directo = 0;
	fcb_nuevo->puntero_indirecto = 0;
	char* path = string_new();
	string_append(&path, path_fcb);
	string_append(&path, "/");
	string_append(&path, nombre);
	string_append(&path, ".dat");
	FILE* f = fopen(path, "wr");
	fprintf(f, "NOMBRE_ARCHIVO=%s\nTAMANIO_ARCHIVO=0\nPUNTERO_DIRECTO=0\nPUNTERO_INDIRECTO=0", nombre);
	fclose(f);


	return fcb_nuevo;
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







