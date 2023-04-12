#include <stdio.h>
#include <stdlib.h>

#include "Memoria.h"


int main(void) {

    memoria_logger = iniciar_logger("./../logs/logMemoria.log", "memoria");
	memoria_config = iniciar_config("./../config/Memoria.config");

	leer_configs(memoria_config, memoria_logger);
	log_info(memoria_logger, "¡Memoria iniciado correctamente!");

	return EXIT_SUCCESS;

}
