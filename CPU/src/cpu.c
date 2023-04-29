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

	conectarMemoria();
	abrirSocketKernel();

	//pthread_create(&atender_kernel, NULL, abrirSocketKernel, NULL);
	//pthread_create(&conexionMemoria, NULL, conectarMemoria, NULL);

	printf("\n\n main \n\n");
	recibir_mensaje_kernel();

	//pthread_detach(conexionMemoria);
	//pthread_join(atender_kernel, NULL);
	//abrirSocketKernel();


	return EXIT_SUCCESS;
}

void* abrirSocketKernel(){
	//while(1){
		socket_Kernel = esperar_cliente(server_cpu, cpu_logger);

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
			printf("\n termino de leer 1° send\n");
			break;
		}
	//}
	return "";
}

void* conectarMemoria(){
	socket_memoria = crear_conexion(ip_memoria, puerto_memoria, cpu_logger, "Memoria");
	handshake(socket_memoria, 1, cpu_logger, "Memoria");

	enviar_mensaje("Soy el CPU", socket_memoria);
	return "";
}

void recibir_mensaje_kernel(){
	printf("\n\n recibiendo mensaje\n\n");
	int cod_op;
	cod_op = recibir_operacion(socket_Kernel);
	printf("\n\n recibi cod_op\n\n");
	printf("\n cod_op: %d \n", cod_op);
	switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(socket_Kernel, cpu_logger);
			break;
		case PAQUETE:
			int size;
			void* buffer;
			int tamanio;
			printf("\n por recibir buffer\n");
			buffer = recibir_buffer(&size, socket_Kernel);
			printf("\n recibi buffer \n");

			memcpy(&tamanio, buffer, sizeof(int));

			t_pcb* pcb_recibido = malloc(tamanio);
			memcpy(pcb_recibido, buffer + sizeof(int), tamanio);

			printf("\n recibi pcb:\n");
			print_pcb(pcb_recibido);
	}
}
