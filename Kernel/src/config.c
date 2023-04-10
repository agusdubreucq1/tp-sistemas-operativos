#include "config.h"


t_config* iniciar_config(char* kernel_config){
	t_config* nuevo_config = config_create(kernel_config);
	if(nuevo_config == NULL){
		printf("No se pudo crear a config de Kernel");
		exit(2);
	}
	return nuevo_config;
}


void leer_configs(t_config* kernel_config, t_log* kernel_logger){
	ip_memoria 						= 	config_get_string_value(kernel_config,"IP_MEMORIA");
	puerto_memoria					=	config_get_string_value(kernel_config,"PUERTO_MEMORIA");
	ip_filesystem 					=	config_get_string_value(kernel_config,"IP_FILESYSTEM");
	puerto_filesystem 				= 	config_get_string_value(kernel_config,"PUERTO_FILESYSTEM");
	ip_cpu 							=	config_get_string_value(kernel_config,"IP_CPU");
	puerto_cpu 						=	config_get_string_value(kernel_config, "PUERTO_CPU");
	algoritmo_planificacion			=	config_get_string_value(kernel_config, "ALGORITMO_PLANIFICACION");
	estimacion_inicial				=	config_get_string_value(kernel_config, "ESTIMACION_INICIAL");
	hrrn_alfa						= 	config_get_string_value(kernel_config, "HRRN_ALFA");
	grado_maximo_multiprogramacion	=	config_get_int_value(kernel_config, "GRADO_MAX_MULTIPROGRAMACION");
	recursos						=	config_get_array_value(kernel_config, "RECURSOS");
	instancias_recursos				=	config_get_array_value(kernel_config, "INSTANCIAS_RECURSOS");

	loggear_configs(kernel_logger);
	loggear_Recursos();

}


void loggear_configs(t_log* kernel_logger){
	log_info(kernel_logger, "-------Valores del config-------");
	log_info(kernel_logger, "IP Memoria = %s", ip_memoria);
	log_info(kernel_logger, "Puerto Memoria = %s", puerto_memoria);
	log_info(kernel_logger, "IP File System = %s", ip_filesystem);
	log_info(kernel_logger, "Puerto File System = %s", puerto_cpu);
	log_info(kernel_logger, "Algoritmo de Planificación = %s", algoritmo_planificacion);
	log_info(kernel_logger, "Estimacion inicial = %s", estimacion_inicial);
	log_info(kernel_logger, "HRRN alfa = %s", hrrn_alfa);
	log_info(kernel_logger, "Grado Máximo de Multiprogramación = %d", grado_maximo_multiprogramacion);
}

void loggear_Recursos(void){

	log_info(kernel_logger, "--------Recursos----------------");

	int i = 0;

	char** ptr = recursos;
	for (char* c = *ptr; c; c=*++ptr) {
		log_info(kernel_logger, "%s %s %s", instancias_recursos[i], " ", c);
		i++;
	}

	log_info(kernel_logger, "--------------------------------");
}

