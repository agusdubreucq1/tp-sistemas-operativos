#include <stdio.h>
#include <stdlib.h>

#include "Memoria.h"


int main(void) {

	memoria_logger = iniciar_logger("../../logs/logMemoria.log", "Memoria");

	if (memoria_logger == NULL){
		exit(1);
	}

	memoria_config = iniciar_config("../../config/Memoria.config", "Memoria");

	if (memoria_config == NULL){
		exit(2);
	}

	leer_configs(memoria_config, memoria_logger);
	log_info(memoria_logger, "¡Memoria iniciado correctamente!");

	server_memoria = iniciar_servidor(IP_SERVER, puerto_escucha, memoria_logger);
	log_info(memoria_logger, "Servidor listo para recibir al cliente");

	pthread_create(&atender_conexiones, NULL, abrirSocket, NULL);
	pthread_join(atender_conexiones, NULL);
	//abrirSocketKernel();

	return EXIT_SUCCESS;
}

void* abrirSocket(){
	while (1) {
		int socket_Kernel = esperar_cliente(server_memoria, memoria_logger);

		uint32_t resultOk = 0;
		uint32_t resultError = -1;

		recv(socket_Kernel, &respuesta, sizeof(uint32_t), MSG_WAITALL);
		if(respuesta == 1)
		   send(socket_Kernel, &resultOk, sizeof(uint32_t), 0);
		else
		   send(socket_Kernel, &resultError, sizeof(uint32_t), 0);

		int cod_op = recibir_operacion(socket_Kernel);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(socket_Kernel, memoria_logger);
			break;
		}
	}
	return "";
}

