/*
 * config.c
 *
 *  Created on: Apr 12, 2023
 *      Author: utnso
 */


#include "config.h"


t_config* iniciar_config(char* kernel_config){
	t_config* nuevo_config = config_create(kernel_config);
	if(nuevo_config == NULL){
		printf("No se pudo crear a config de Kernel");
		exit(2);
	}
	return nuevo_config;
}
