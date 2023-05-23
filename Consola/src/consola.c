#include "consola.h"

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>


int main(int argc, char** argv) {

    if (argc != 3) {
    	puts("\n No se introdujo la cantidad de argumentos necesarios"
    	        "\n   * Primer  argumento: <Archivo de configuracion>"
               "\n   * Segundo argumento: <Archivo de pseudocodigo>");
    	return EXIT_FAILURE;
    }

    consola_logger = iniciar_logger("../../logs/logConsola.log", "Consola");

	if (consola_logger == NULL){
		exit(1);
	}

	consola_config = iniciar_config(argv[1], "Consola");

	if (consola_config == NULL){
		exit(2);
	}

	log_info(consola_logger, "¡Consola iniciada correctamente!");

	leer_configs(consola_logger, consola_config);

	buffer = parsearInstrucciones(argv[2]);

	conexion_kernel = crear_conexion(ip_kernel, puerto_kernel, consola_logger, "Kernel");
	handshake(conexion_kernel, 1, consola_logger, "Kernel");

	paquete = crear_paquete();

	agregar_a_paquete(paquete, buffer, strlen(buffer) + 1);

	enviar_paquete(paquete, conexion_kernel, consola_logger, "Kernel");

	terminar_programa(conexion_kernel, consola_logger, consola_config);

	return EXIT_SUCCESS;

}

void terminar_programa(int conexion, t_log *logger, t_config* config){
	while(conexion == 1){

	}
	log_info(logger, "Entro a finalizar el programa");
	log_destroy(logger);
	config_destroy(config);
	close(conexion);
}
