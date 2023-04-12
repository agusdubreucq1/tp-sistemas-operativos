/*
 * filesystem.c
 *
 *  Created on: Apr 12, 2023
 *      Author: utnso
 */

#include "fileSystem.h"


int main(void){

	fileSystem_logger = iniciar_logger("./../logs/logFileSystem.log", "FileSystem");
	fileSystem_config = iniciar_config("./../config/FileSystem.config");

	log_info(fileSystem_logger, "¡Kernel iniciado correctamente!");

	return EXIT_SUCCESS;
}
