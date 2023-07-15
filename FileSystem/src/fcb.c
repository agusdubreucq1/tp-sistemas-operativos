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
	fprintf(f, "NOMBRE_ARCHIVO=%s\nTAMANIO_ARCHIVO=0\nPUNTERO_DIRECTO=\nPUNTERO_INDIRECTO=", nombre);
	fclose(f);


	return fcb_nuevo;
}

void grabar_fcb(t_fcb* fcb){
	char* path = string_new();
	string_append(&path, path_fcb);
	string_append(&path, "/");
	string_append(&path, fcb->nombre_archivo);
	string_append(&path, ".dat");
	FILE* f = fopen(path, "wr");
	fprintf(f, "NOMBRE_ARCHIVO=%s\nTAMANIO_ARCHIVO=%d\nPUNTERO_DIRECTO=%d\nPUNTERO_INDIRECTO=%d", fcb->nombre_archivo, fcb->tamano_archivo, fcb->puntero_directo, fcb->puntero_indirecto);
	fclose(f);
	free(path);
}


/*void inicializar_fcbs(){
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
}*/

void inicializar_FCBs(){
	lista_fcb = list_create();
	DIR *d;
	struct dirent *dir;
	    d = opendir(path_fcb);
	    if (d) {
	        while ((dir = readdir(d)) != NULL) {
	        	if (dir->d_type == DT_REG) {
					char* path = string_new();
					string_append(&path, path_fcb);
					string_append(&path, "/");
					string_append(&path, dir->d_name);

					t_config* fcb_config = config_create(path);
					char* nombre = config_get_string_value(fcb_config, "NOMBRE_ARCHIVO");
					uint32_t tamanio = config_get_int_value(fcb_config, "TAMANIO_ARCHIVO");
					uint32_t puntero_directo = config_get_int_value(fcb_config, "PUNTERO_DIRECTO");
					uint32_t puntero_indirecto = config_get_int_value(fcb_config, "PUNTERO_INDIRECTO");
					//printf("----------\n-nombre: %s\n-tamanio: %d\n-pd: %d\n-pi: %d\n----------\n\n", nombre, tamanio, puntero_directo, puntero_indirecto);

					t_fcb* fcb = leer_fcb(nombre, tamanio, puntero_directo, puntero_indirecto);
					list_add(lista_fcb, fcb);
					config_destroy(fcb_config);
					free(path);
	        	}
	        }
	        closedir(d);
	    }
}

t_fcb* leer_fcb(char* nombre, uint32_t tamanio, uint32_t puntero_directo, uint32_t puntero_indirecto){
	t_fcb* fcb;
	fcb = malloc(sizeof(t_fcb));
	fcb->nombre_archivo = string_new();
	string_append(&fcb->nombre_archivo, nombre);
	fcb->tamano_archivo = tamanio;
	fcb->puntero_directo = puntero_directo;
	fcb->puntero_indirecto = puntero_indirecto;
	return fcb;
}

t_fcb* fcb_segun_nombre(char* archivo){
	for(int i=0;i<list_size(lista_fcb);i++){
		t_fcb* fcb = list_get(lista_fcb, i);
		if(string_equals_ignore_case(fcb->nombre_archivo, archivo)){
			return fcb;
		}
	}
	return NULL;
}


