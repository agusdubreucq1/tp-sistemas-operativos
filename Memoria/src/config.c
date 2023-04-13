#include "config.h"

t_config* iniciar_config(char* path_config){
	t_config* nuevo_config = config_create(path_config);
		if(nuevo_config == NULL){
			printf("No se pudo crear a config de memoria");
			exit(2);
		}
		return nuevo_config;
}

void leer_configs(t_config* memoria_config, t_log* memoria_logger){
	puerto_escucha				=   config_get_string_value(memoria_config,"PUERTO_ESCUCHA");
	tam_memoria					=	config_get_string_value(memoria_config,"TAM_MEMORIA");
	tam_segmento_0				=	config_get_string_value(memoria_config,"TAM_SEGMENTO_0");
	cant_segmentos				= 	config_get_string_value(memoria_config,"CANT_SEGMENTOS");
	retardo_memoria 			=	config_get_string_value(memoria_config,"RETARDO_MEMORIA");
	retardo_compactacion 		=	config_get_string_value(memoria_config, "RETARDO_COMPACTACION");
	algoritmo_asignacion		=	config_get_string_value(memoria_config, "ALGORITMO_ASIGNACION");

	loggear_configs(memoria_logger);


}

void loggear_configs(t_log* memoria_logger){
	log_info(memoria_logger, "-------Valores del config-------");
	log_info(memoria_logger, "puerto_escucha = %s", puerto_escucha);
	log_info(memoria_logger, "tamaño memoria = %s", tam_memoria);
	log_info(memoria_logger, "tamaño segmento = %s", tam_segmento_0);
	log_info(memoria_logger, "cantidad de segmentos = %s", cant_segmentos);
	log_info(memoria_logger, "retardo memoria = %s", retardo_memoria);
	log_info(memoria_logger, "retardo compactacion = %s", retardo_compactacion);
	log_info(memoria_logger, "algoritmo asignacion = %s", algoritmo_asignacion);

	log_info(memoria_logger, "--------------------------------");
}
