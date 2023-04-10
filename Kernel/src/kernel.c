#include "kernel.h"
#include "logger.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){


	// ------------------------------------------------------------------- Configs

	kernel_config = iniciar_config("../../config/ker");
	kernel_logger = iniciar_logger(argv[2], "Kernel");

	leer_configs(kernel_config, kernel_logger);


	log_info(kernel_logger, "¡Kernel iniciado correctamente!");


	return EXIT_SUCCESS;
}
