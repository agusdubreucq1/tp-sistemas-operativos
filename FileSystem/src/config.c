#include "config.h"


t_config* iniciar_config(char* file_config){
	t_config* nuevo_config = config_create(file_config);
	if(nuevo_config == NULL){
		printf("No se pudo crear a config de File System");
		exit(2);
	}
	return nuevo_config;
}

void leer_configs(t_config* file_config, t_log* file_logger){
	file_ip_memoria 				= 	config_get_string_value(file_config,"IP_MEMORIA");
	puerto_memoria					=	config_get_string_value(file_config,"PUERTO_MEMORIA");
	puerto_escucha 					=	config_get_string_value(file_config,"PUERTO_ESCUCHA");
	path_superbloque 				= 	config_get_string_value(file_config,"PATH_SUPERBLOQUE");
	file_path_bitmap 					=	config_get_string_value(file_config,"PATH_BITMAP");
	path_bloques					=	config_get_string_value(file_config, "PATH_BLOQUES");
	path_fcb	 					=	config_get_string_value(file_config, "PATH_FCB");
	retardo_acceso_bloque			=	config_get_string_value(file_config, "RETARDO_ACCESO_BLOQUE");

	loggear_configs(file_logger);

}

void loggear_configs(t_log* file_logger){
	log_info(file_logger, "-------Valores del config-------");
	log_info(file_logger, "IP Memoria = %s", file_ip_memoria);
	log_info(file_logger, "Puerto Memoria = %s", puerto_memoria);
	log_info(file_logger, "Puerto Escucha = %s", puerto_escucha);
	log_info(file_logger, "Path Superbloque = %s", path_superbloque);
	log_info(file_logger, "Path Bitmap = %s", file_path_bitmap);
	log_info(file_logger, "Path Bloques = %s", path_bloques);
	log_info(file_logger, "Path FCB = %s", path_fcb);
	log_info(file_logger, "Retardo Acceso Bloque = %s", retardo_acceso_bloque);

}
