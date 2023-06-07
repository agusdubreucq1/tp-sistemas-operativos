#include "config.h"

void leer_configs(t_config* cpu_config, t_log* cpu_logger){
	retardo_instruccion = 	config_get_int_value(cpu_config, "RETARDO_INSTRUCCION");
	ip_memoria 			= 	config_get_string_value(cpu_config,"IP_MEMORIA");
	puerto_memoria 		= 	config_get_string_value(cpu_config, "PUERTO_MEMORIA");
	puerto_escucha	 	= 	config_get_string_value(cpu_config,"PUERTO_ESCUCHA");
	tam_max_segmento	= 	config_get_string_value(cpu_config, "TAM_MAX_SEGMENTO");
	loggear_configs(cpu_logger);
}

void loggear_configs(t_log* cpu_logger){
	log_info(cpu_logger, "-------Valores del config-------");
	log_info(cpu_logger, "Retardo Instrucción = %d", retardo_instruccion);
	log_info(cpu_logger, "IP Memoria = %s", ip_memoria);
	log_info(cpu_logger, "Puerto Memoria = %s", puerto_memoria);
	log_info(cpu_logger, "Puerto Escucha = %s", puerto_escucha);
	log_info(cpu_logger, "Tamanio maximo de segmento = %s", tam_max_segmento);
	log_info(cpu_logger, "--------------------------------");
}
