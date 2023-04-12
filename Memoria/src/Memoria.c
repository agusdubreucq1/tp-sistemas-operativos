#include <stdio.h>
#include <stdlib.h>

#include "Memoria.h"


int main(void) {

    memoria_logger = iniciar_logger("../../logs/logMemoria.log", "memoria");
	memoria_config = iniciar_config("../../config/Memoria.config");


	leer_configs(memoria_config, memoria_logger);

	return EXIT_SUCCESS;

}
