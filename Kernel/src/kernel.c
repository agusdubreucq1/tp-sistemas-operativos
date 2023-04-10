#include "kernel.h"
#include "logger.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){


	// ------------------------------------------------------------------- Configs

    if (argc != 3) {
        puts("\n No se introdujo la cantidad de argumentos necesarios"
                "\n   * Primer  argumento: <Archivo de configuracion>"
                "\n   * Segundo argumento: <Archivo de log>");
        return EXIT_FAILURE;
    }

	kernel_config = iniciar_config(argv[1]);
	kernel_logger = iniciar_logger(argv[2], "Kernel");

	leer_configs(kernel_config, kernel_logger);


	log_info(kernel_logger, "¡Kernel iniciado correctamente!");


	return EXIT_SUCCESS;
}
