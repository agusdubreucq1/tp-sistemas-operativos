#include "cpu.h"

int main(int argc, char** argv){


    cpu_logger = iniciar_logger("../../logs/logCPU.log", "CPU");
    cpu_config = iniciar_config("../../config/CPU.config");

    leer_configs(cpu_config, cpu_logger);
    log_info(cpu_logger, "¡CPU iniciado correctamente!");

	server_cpu = iniciar_servidor();
	log_info(cpu_logger, "Servidor listo para recibir al cliente");

	abrirSocketKernel();

	return EXIT_SUCCESS;
}

void abrirSocketKernel(){
	int socket_Kernel = esperar_cliente(server_cpu);

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
