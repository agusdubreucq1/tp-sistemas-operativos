#include "cpu.h"

int main(void){

	signal(SIGINT, cerrar_conexiones);

	cpu_logger = iniciar_logger("../logs/logCPU.log", "CPU");

	if (cpu_logger == NULL){
		exit(1);
	}

	cpu_config = iniciar_config("../config/CPU.config", "CPU");

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
	while(1){
		recibir_mensaje_kernel();
	}


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

			t_contexto_ejecucion* contexto_recibido = deserializar_contexto(buffer, tam_recibido);

			*tam_recibido+=2*sizeof(int);
			printf("\n tamanio recibido: %d\n", *tam_recibido);
			printf("puntero: %p\n", tam_recibido);
			int var_send_ = send(socket_Kernel, tam_recibido, sizeof(int), 0);
			printf("var_send: %d\n", var_send_);

			printf("\n recibi contexto:\n");
			print_contexto(contexto_recibido);
	}
}

t_contexto_ejecucion* deserializar_contexto(void* stream,int* bytes_recibidos){
	t_contexto_ejecucion* contexto_deserializado= malloc(sizeof(t_contexto_ejecucion));
	int desplazamiento = 0;

	contexto_deserializado->pid = deserializar_uint32(stream, &desplazamiento);

	contexto_deserializado->instrucciones = deserializar_instrucciones(stream, &desplazamiento);

	contexto_deserializado->program_counter = deserializar_uint32(stream, &desplazamiento);

	contexto_deserializado->registros_cpu =  deserializar_registros_cpu(stream, &desplazamiento);
	print_registos(contexto_deserializado->registros_cpu);


	contexto_deserializado->tabla_segmentos = deserializar_tabla_segmentos(stream, &desplazamiento);
	print_segmento(contexto_deserializado->tabla_segmentos);

	*bytes_recibidos = desplazamiento;

	return contexto_deserializado;
}

 t_registros* deserializar_registros_cpu(void* stream, int* desplazamiento){
	t_registros* registros=malloc(sizeof(t_registros));

		strncpy(registros->ax, deserializar_char(stream, desplazamiento, 4), 4);
		strncpy(registros->bx, deserializar_char(stream, desplazamiento, 4),4);
		strncpy(registros->cx, deserializar_char(stream, desplazamiento, 4), 4);
		strncpy(registros->dx, deserializar_char(stream, desplazamiento, 4), 4);
		strncpy(registros->eax, deserializar_char(stream, desplazamiento, 8), 8); /*-> usar strncpy, sino se copiara hasta encontrar un \0*/
		strncpy(registros->ebx, deserializar_char(stream, desplazamiento, 8), 8);
		strncpy(registros->ecx, deserializar_char(stream, desplazamiento, 8), 8);
		strncpy(registros->edx, deserializar_char(stream, desplazamiento, 8), 8);
		strncpy(registros->rax, deserializar_char(stream, desplazamiento, 16), 16);
		strncpy(registros->rbx, deserializar_char(stream, desplazamiento, 16), 16);
		strncpy(registros->rcx, deserializar_char(stream, desplazamiento, 16), 16);
		strncpy(registros->rdx, deserializar_char(stream, desplazamiento, 16), 16);


	return registros;
}

t_list* deserializar_tabla_segmentos(void* stream, int* desplazamiento){
	t_list* tabla_segmentos= list_create();
	int cant_segmentos = deserializar_uint32(stream, desplazamiento);
	int tamanio;

	for(int j=0; j<cant_segmentos; j++){
			t_segmento segmento;
			memcpy(&tamanio, stream + *desplazamiento, sizeof(int));
			*desplazamiento += sizeof(int);
			memcpy(&segmento, stream + *desplazamiento, sizeof(t_segmento));
			*desplazamiento += sizeof(t_segmento);
			list_add(tabla_segmentos, &segmento);
		}
	return tabla_segmentos;
}

 char* deserializar_char(void* stream, int* desplazamiento, int tamanio){
	 //int tamanio;
	 char* registro;

	 /*memcpy(&tamanio, stream + *desplazamiento, sizeof(int));
	 *desplazamiento += sizeof(int);*/
	 registro = stream + *desplazamiento;
	 *desplazamiento+=tamanio;
	 return registro;

 }

uint32_t deserializar_uint32(void* stream, int* desplazamiento){
	uint32_t* variable;
	//uint32_t tamanio;

	/*memcpy(&tamanio, stream + *desplazamiento, sizeof(uint32_t));
	*desplazamiento+= sizeof(uint32_t);*/
	variable = stream + *desplazamiento;
	*desplazamiento+= sizeof(uint32_t);

	return *variable;
}

t_list* deserializar_instrucciones(void* stream, int* desplazamiento){
	int cant_instrucciones = deserializar_uint32(stream, desplazamiento);
	int tamanio;
	t_list* lista_instrucciones= list_create();
	for(int i=0; i<cant_instrucciones; i++){
			char* instruccion;
			memcpy(&tamanio, stream + *desplazamiento, sizeof(int));
			*desplazamiento+=sizeof(int);
			instruccion = malloc(tamanio);
			memcpy(instruccion, stream + *desplazamiento, tamanio);
			*desplazamiento+=tamanio;
			instruccion[tamanio-1]='\0';
			printf("instruccion: %s \n", instruccion);

			list_add(lista_instrucciones, instruccion);
		}
	return lista_instrucciones;
}

void cerrar_conexiones(){
	printf("\ncerrando conexiones\n");

	close(server_cpu);
	close(socket_Kernel);
	close(socket_memoria);
printf("cerre conexiones");
	exit(1);
}
