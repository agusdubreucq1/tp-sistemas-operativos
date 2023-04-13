#include <stdio.h>
#include <stdlib.h>

#include "Memoria.h"


int main(void) {

    memoria_logger = iniciar_logger("../../logs/logMemoria.log", "memoria");
	memoria_config = iniciar_config("../../config/Memoria.config");

	leer_configs(memoria_config, memoria_logger);
	log_info(memoria_logger, "¡Memoria iniciado correctamente!");

	server_memoria = iniciar_servidor();
	log_info(memoria_logger, "Servidor listo para recibir al cliente");

	abrirSocketKernel();

	return EXIT_SUCCESS;
}

void abrirSocketKernel(){
	int socket_Kernel = esperar_cliente(server_memoria);

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
}
