#include "cpu.h"

int main(int argc, char** argv){


    cpu_logger = iniciar_logger("../../logs/logCPU.log", "CPU");
    cpu_config = iniciar_config("../../config/CPU.config");

    leer_configs(cpu_config, cpu_logger);
    log_info(cpu_logger, "¡CPU iniciado correctamente!");

	server_cpu = iniciar_servidor();
	log_info(cpu_logger, "Servidor listo para recibir al cliente");

	pthread_create(&atender_kernel, NULL, abrirSocketKernel, NULL);
	pthread_create(&conexionMemoria, NULL, conectarMemoria, NULL);

	pthread_detach(conexionMemoria);
	pthread_join(atender_kernel, NULL);
	//abrirSocketKernel();

	return EXIT_SUCCESS;
}

void* abrirSocketKernel(){
	int socket_Kernel = esperar_cliente(server_cpu);

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
		recibir_mensaje(socket_Kernel);
		break;
	}
	return "";
}

void* conectarMemoria(){
	socket_memoria = crear_conexion(ip_memoria, puerto_memoria);
	respuesta = handshake(socket_memoria);
	if(respuesta == 0) {
		log_info(cpu_logger, "Conexion establecida con la Memoria");
	} else {
		log_error(cpu_logger, "Error en la conexión con la Memoria");
		return "";
	}

	enviar_mensaje("Soy el CPU", socket_memoria);
	return "";
}
