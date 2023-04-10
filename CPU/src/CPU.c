#include "CPU.h"

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {
	t_log* logCPU;
	t_config* configCPU;


	logCPU = iniciar_logger();

	if(argc !=3){
		printf("\n no se introdujo la cantidad de archivos necesarios\n");
		log_info(logCPU, "\n cantidad de archivos incorrecta\n");
		return EXIT_FAILURE;
	}

	configCPU = iniciar_config(argv[1]);

	leer_config(logCPU, configCPU);

	terminar_programa(logCPU, configCPU);


	return EXIT_SUCCESS;
}

t_log* iniciar_logger(void){
	t_log* nuevo_logger;

		if ((nuevo_logger = log_create("./../logs/CPU.log", "LOG CONSOLA", 1, LOG_LEVEL_INFO)) == NULL){
			printf("No pude crear el logger\n");
			exit(1);
		}

		return nuevo_logger;
}

t_config* iniciar_config(char* ruta_config){
	t_config* nuevo_config = config_create(ruta_config);
			if(nuevo_config == NULL){
				printf("No se pudo crear a config de la Consola");
				exit(2);
			}
			return nuevo_config;
}

void leer_config(t_log* logger, t_config* config){
		char* ip_kernel;
		char* puerto_kernel;

		ip_kernel = config_get_string_value(config, "IP_KERNEL");
		puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");

		log_info(logger, "-------Valores del config-------");

		log_info(logger, "Ip = %s", ip_kernel);
		log_info(logger, "Puerto = %s", puerto_kernel);

		log_info(logger, "--------------------------------");

}

void terminar_programa(t_log* logger, t_config* config){
		log_info(logger, "fin del programa");
		log_destroy(logger);
		config_destroy(config);
}
