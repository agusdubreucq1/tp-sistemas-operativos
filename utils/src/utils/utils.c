/*
 * utils.c
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */


#include "utils.h"


// ------------------------------------------------------------------------------------------
// -- Logger --
// ------------------------------------------------------------------------------------------


t_log* iniciar_logger(char* path, char* nombre_modulo){
	t_log* logger = log_create(path, nombre_modulo, 1, LOG_LEVEL_TRACE);
	if(logger == NULL){
		printf("No se ha podido crear el logger de %s\n", nombre_modulo);
		return NULL;
	}
	log_info(logger,"Logger creado");
	return logger;
}

// ------------------------------------------------------------------------------------------
// -- Config --
// ------------------------------------------------------------------------------------------

t_config* iniciar_config(char* config, char* nombre_modulo){
	t_config* nuevo_config = config_create(config);
	if(nuevo_config == NULL){
		printf("No se pudo crear a config de %s\n", nombre_modulo);
		return NULL;
	}
	return nuevo_config;
}

// ------------------------------------------------------------------------------------------
// -- Lista --
// ------------------------------------------------------------------------------------------

void iterator(char* value) {
	printf("%s", value);
}