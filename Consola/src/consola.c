#include "consola.h"

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>

int main(int argc, char** argv) {

    if (argc != 2) {
    	puts("\n No se introdujo la cantidad de argumentos necesarios"
    	        "\n   * Primer  argumento: <Archivo de configuracion>"
                "\n   * Segundo argumento: <Archivo de log>");
    	//		"\n   * Tercer  argumento: <Archivo de pseudocodigo>"); Luego se va a implementar
    	return EXIT_FAILURE;
    }

	consola_config = iniciar_config(argv[1]);
    consola_logger = iniciar_logger(argv[2], "Consola");

	log_info(consola_logger, "¡Consola iniciada correctamente!");

	leer_configs(consola_logger, consola_config);

}
