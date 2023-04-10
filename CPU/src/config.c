#include "config.h"

t_config* iniciar_config(char* cpu_config){
	t_config* nuevo_config = config_create(cpu_config);
		if(nuevo_config == NULL){
			printf("No se pudo crear el config del CPU");
			//exit(2);
		}
		return nuevo_config;
}

void leer_configs(t_config* cpu_config){
	retardo_instruccion = 	config_get_int_value(cpu_config, "RETARDO_INSTRUCCION");
	ip_memoria 			= 	config_get_string_value(cpu_config,"IP_MEMORIA");
	puerto_memoria 		= 	config_get_string_value(cpu_config, "PUERTO_MEMORIA");
	puerto_escucha	 	= 	config_get_string_value(cpu_config,"PUERTO_ESCUCHA");
	tam_max_segmentos	= 	config_get_int_value(cpu_config, "TAM_MAX_SEGMENTOS");

	loggear_configs();
}

void loggear_configs(){
	log_info(cpu_logger, "-------Valores del config-------");
	log_info(cpu_logger, "Retardo Instrucción = %d", retardo_instruccion);
	log_info(cpu_logger, "IP Memoria = %s", ip_memoria);
	log_info(cpu_logger, "Puerto Memoria = %s", puerto_memoria);
	log_info(cpu_logger, "Puerto Escucha = %s", puerto_escucha);
	log_info(cpu_logger, "Tamanio maximo de segmentos = %d", tam_max_segmentos);
	log_info(cpu_logger, "--------------------------------");
}
