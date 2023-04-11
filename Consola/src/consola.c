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

	consola_config = iniciar_config(argv[1]);
	consola_logger = iniciar_logger("../../logs/logConsola.log", "Consola");

	log_info(consola_logger, "¡Consola iniciada correctamente!");

	leer_configs(consola_logger, consola_config);

	buffer = parsearInstrucciones(argv[2]);

	conexion_kernel = crear_conexion(ip_kernel, puerto_kernel);
	handshake(conexion_kernel);

	paquete = crear_paquete();

	agregar_a_paquete(paquete, buffer, strlen(buffer) + 1);

	enviar_paquete(paquete, conexion_kernel, consola_logger);
	eliminar_paquete(paquete);

	terminar_programa(conexion_kernel, consola_logger, consola_config);

	return EXIT_SUCCESS;

}

void terminar_programa(int conexion, t_log *logger, t_config* config){
	log_info(logger, "Entro a finalizar el programa");
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
