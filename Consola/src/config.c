#include "config.h"

t_config* iniciar_config(char* ruta_config){
	t_config* nuevo_config = config_create(ruta_config);
		if(nuevo_config == NULL){
			printf("No se pudo crear a config de la Consola");
			exit(2);
		}
		return nuevo_config;
}

void leer_configs(t_log* consola_logger, t_config* config){
	ip_kernel = config_get_string_value(config, "IP_KERNEL");
	puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");

	loggear_valores_config(consola_logger);

}

void loggear_valores_config(t_log* consola_logger){

	log_info(consola_logger, "-------Valores del config-------");

	log_info(consola_logger, "Ip = %s", ip_kernel);
	log_info(consola_logger, "Puerto = %s", puerto_kernel);

	log_info(consola_logger, "--------------------------------");
}
