#include "kernel.h"
#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/string.h>

int main(void){


	// ------------------------------------------------------------------- Configs

	kernel_logger = iniciar_logger("./../logs/logKernel.log", "Kernel");
	kernel_config = iniciar_config("./../config/Kernel.config");

	leer_configs(kernel_config, kernel_logger);

	log_info(kernel_logger, "¡Kernel iniciado correctamente!");


	return EXIT_SUCCESS;
}
