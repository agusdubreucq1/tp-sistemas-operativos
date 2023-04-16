#include "cpu.h"

int main(void){

	cpu_logger = iniciar_logger("../../logs/logCPU.log", "CPU");

	if (cpu_logger == NULL){
		exit(1);
	}

	cpu_config = iniciar_config("../../config/CPU.config", "CPU");

	if (cpu_config == NULL){
		exit(2);
	}

    leer_configs(cpu_config, cpu_logger);
    log_info(cpu_logger, "¡CPU iniciado correctamente!");

	server_cpu = iniciar_servidor(IP_SERVER, puerto_escucha, cpu_logger);
	log_info(cpu_logger, "Servidor listo para recibir al cliente");

	pthread_create(&atender_kernel, NULL, abrirSocketKernel, NULL);
	pthread_create(&conexionMemoria, NULL, conectarMemoria, NULL);

	pthread_detach(conexionMemoria);
	pthread_join(atender_kernel, NULL);
	//abrirSocketKernel();

	return EXIT_SUCCESS;
}

void* abrirSocketKernel(){
	int socket_Kernel = esperar_cliente(server_cpu, cpu_logger);

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
		recibir_mensaje(socket_Kernel, cpu_logger);
		break;
	}
	return "";
}

void* conectarMemoria(){
	socket_memoria = crear_conexion(ip_memoria, puerto_memoria, cpu_logger, "Memoria");
	handshake(socket_memoria, 1, cpu_logger, "Memoria");

	enviar_mensaje("Soy el CPU", socket_memoria);
	return "";
}
