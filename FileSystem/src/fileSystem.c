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

	leer_configs(fileSystem_config, fileSystem_logger);
	log_info(fileSystem_logger, "¡File System iniciado correctamente!");

	server_fileSystem = iniciar_servidor();
	log_info(fileSystem_logger, "Servidor listo para recibir al cliente");

	int socket_Kernel = esperar_cliente(server_fileSystem);

	uint32_t resultOk = 0;
	uint32_t resultError = -1;

	recv(socket_Kernel, &handshake, sizeof(uint32_t), MSG_WAITALL);
	if(handshake == 1)
	   send(socket_Kernel, &resultOk, sizeof(uint32_t), 0);
	else
	   send(socket_Kernel, &resultError, sizeof(uint32_t), 0);

	int cod_op = recibir_operacion(socket_Kernel);
	switch (cod_op) {
	case MENSAJE:
		recibir_mensaje(socket_Kernel);
		break;
	}

	return EXIT_SUCCESS;
}
