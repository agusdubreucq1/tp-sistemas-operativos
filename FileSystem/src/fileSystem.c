/*
 * filesystem.c
 *
 *  Created on: Apr 12, 2023
 *      Author: utnso
 */

#include "fileSystem.h"


int main(void){

	fileSystem_logger = iniciar_logger("../../logs/logFileSystem.log", "FileSystem");
	fileSystem_config = iniciar_config("../../config/FileSystem.config");

	log_info(fileSystem_logger, "¡Kernel iniciado correctamente!");

	leer_configs(fileSystem_config, fileSystem_logger);

	return EXIT_SUCCESS;
}
