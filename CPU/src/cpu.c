#include "cpu.h"

int main(void){

	//signal(SIGINT, cerrar_conexiones);

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

	//iniciar_servidor_cpu();

	server_cpu = iniciar_servidor(IP_SERVER, puerto_escucha, cpu_logger);
	log_info(cpu_logger, "Servidor listo para recibir al cliente");

	contexto_de_ejecucion =  malloc(sizeof(t_contexto_ejecucion));

	pthread_create(&atender_kernel, NULL, abrirSocketKernel, NULL);
	pthread_create(&conexionMemoria, NULL, conectarMemoria, NULL);

	pthread_detach(conexionMemoria);
	pthread_join(atender_kernel, NULL);
	//abrirSocketKernel();



	return EXIT_SUCCESS;
}

void* abrirSocketKernel(){

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
			break;
		}

		while(1){
			recibir_mensaje_kernel();
			comenzar_ciclo_instruccion(contexto_de_ejecucion);


			// ACA INTERPRETAMOS LAS INSTRUCCIONES
			//printf("/n/n/n/n/n/ ASASASASASASASASSASASASASASAS /n/n/n/n/n/n/n/n/n/n/n");
			//
			//enviarContexto();

		}

	return "";
}

void* conectarMemoria(){
	socket_memoria = crear_conexion(ip_memoria, puerto_memoria, cpu_logger, "Memoria");
	handshake(socket_memoria, 1, cpu_logger, "Memoria");

	enviar_mensaje("Soy el CPU", socket_memoria);
	return "";
}


void recibir_mensaje_kernel(){
	int cod_op;
	cod_op = recibir_operacion(socket_Kernel);
	printf("\n cod_op: %d \n", cod_op);
	switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(socket_Kernel, cpu_logger);
			break;
		case PAQUETE:
			int size;
			void* buffer;
			int* tam_recibido= malloc(sizeof(int));
			buffer = recibir_buffer(&size, socket_Kernel);
			printf("\n recibi buffer \n");

			contexto_de_ejecucion = deserializar_pcb(buffer, tam_recibido);

			*tam_recibido+=2*sizeof(int);
			printf("\n tamanio recibido: %d\n", *tam_recibido);
			//printf("puntero: %p\n", tam_recibido);
			int var_send_ = send(socket_Kernel, tam_recibido, sizeof(int), 0);
			//printf("var_send: %d\n", var_send_);

			printf("\n recibi contexto:\n");
			//print_contexto(contexto_de_ejecucion);
	}
}


void cerrar_conexiones(){
	printf("\ncerrando conexiones\n");

	close(server_cpu);
	//close(socket_Kernel);
	close(socket_memoria);
	printf("cerre conexiones");
	exit(1);
}
