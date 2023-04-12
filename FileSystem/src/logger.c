/*
 * logger.c
 *
 *  Created on: Apr 12, 2023
 *      Author: utnso
 */

#include "logger.h"


t_log* iniciar_logger(char* path, char* nombre_modulo){
	t_log* nuevo_logger = log_create(path, nombre_modulo, 1, LOG_LEVEL_TRACE);
	if(nuevo_logger == NULL){
		printf("No se ha podido crear el logger de %s\n", nombre_modulo);
		exit(1);
	}
	log_info(nuevo_logger,"Logger creado");
	return nuevo_logger;
}
