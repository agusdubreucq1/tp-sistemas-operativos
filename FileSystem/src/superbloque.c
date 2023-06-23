/*
 * superbloque.c
 *
 *  Created on: Jun 13, 2023
 *      Author: utnso
 */

#include "superbloque.h"

void levantar_superBloque(char* path, t_log* logger){
	t_config* configSuperBloque = config_create(path);
	tamanio_bloque = config_get_int_value(configSuperBloque, "BLOCK_SIZE");
	cant_bloques = config_get_int_value(configSuperBloque, "BLOCK_COUNT");
	log_trace(logger, "Super Bloque Montado");
	log_trace(logger, "Tamaño de Bloque: %d", tamanio_bloque);
	log_trace(logger, "Cantidad de Bloques: %d", cant_bloques);
	config_destroy(configSuperBloque);
}


int bloques_necesarios(int bytes){
	return redondearArriba(bytes/(float)tamanio_bloque);
}

int redondearArriba(double x){
	int i = (int)x;
	return i + (x > i);
}
