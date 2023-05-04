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
	int cant_instrucciones;
	int cant_segmentos;

	contexto_deserializado->pid = deserializar_uint32(stream, &desplazamiento);

	cant_instrucciones = deserializar_uint32(stream, &desplazamiento);

	contexto_deserializado->instrucciones = deserializar_instrucciones(stream, cant_instrucciones, &desplazamiento);

	contexto_deserializado->program_counter = deserializar_uint32(stream, &desplazamiento);


	contexto_deserializado->registros_cpu =  deserializar_registros_cpu(stream, &desplazamiento);
	print_registos(contexto_deserializado->registros_cpu);

	cant_segmentos = deserializar_uint32(stream, &desplazamiento);

	contexto_deserializado->tabla_segmentos = deserializar_tabla_segmentos(stream, &desplazamiento, cant_segmentos);
	print_segmento(contexto_deserializado->tabla_segmentos);

	*bytes_recibidos = desplazamiento;

	return contexto_deserializado;
}

 t_registros* deserializar_registros_cpu(void* stream, int* desplazamiento){
	//int tamanio=0;
	t_registros* registros=malloc(sizeof(t_registros));

		strncpy(registros->ax, deserializar_char(stream, desplazamiento), 4);
		strncpy(registros->bx, deserializar_char(stream, desplazamiento),4);
		strncpy(registros->cx, deserializar_char(stream, desplazamiento), 4);
		strncpy(registros->dx, deserializar_char(stream, desplazamiento), 4);
		strncpy(registros->eax, deserializar_char(stream, desplazamiento), 8); /*-> usar strncpy, sino se copiara hasta encontrar un \0*/
		strncpy(registros->ebx, deserializar_char(stream, desplazamiento), 8);
		strncpy(registros->ecx, deserializar_char(stream, desplazamiento), 8);
		strncpy(registros->edx, deserializar_char(stream, desplazamiento), 8);
		strncpy(registros->rax, deserializar_char(stream, desplazamiento), 16);
		strncpy(registros->rbx, deserializar_char(stream, desplazamiento), 16);
		strncpy(registros->rcx, deserializar_char(stream, desplazamiento), 16);
		strncpy(registros->rdx, deserializar_char(stream, desplazamiento), 16);
	//char*[12]=[registros->ax, ]

	/*memcpy(&tamanio, stream + *desplazamiento, sizeof(int));
	*desplazamiento += sizeof(int);
	memcpy(registros->ax, stream + *desplazamiento, tamanio);
	*desplazamiento += tamanio;
	strcpy(registros->ax, deserializar_char(stream, desplazamiento));
	strcpy(registros->bx, deserializar_char(stream, desplazamiento));
	strcpy(registros->cx, deserializar_char(stream, desplazamiento));
	strcpy(registros->dx, deserializar_char(stream, desplazamiento));
	strcpy(registros->eax, deserializar_char(stream, desplazamiento)); -> usar strncpy, sino se copiara hasta encontrar un \0
	strcpy(registros->ebx, deserializar_char(stream, desplazamiento));
	strcpy(registros->ecx, deserializar_char(stream, desplazamiento));
	strcpy(registros->edx, deserializar_char(stream, desplazamiento));
	strcpy(registros->rax, deserializar_char(stream, desplazamiento));
	strcpy(registros->rbx, deserializar_char(stream, desplazamiento));
	strcpy(registros->rcx, deserializar_char(stream, desplazamiento));
	strcpy(registros->rdx, deserializar_char(stream, desplazamiento)); -> si deserializar char hace malloc, no estamos liberando memeoria,
	y si hace un char[], luego de ejecutar la funcion deserializar la variable deja d existir por lo que otro proceso/hilo podria escribir
	en esa memoria*/

	/*registros->bx = deserializar_char(stream, desplazamiento);
	registros->cx = deserializar_char(stream, desplazamiento);
	registros->dx = deserializar_char(stream, desplazamiento);
	registros->eax = deserializar_char(stream, desplazamiento);
	registros->ebx = deserializar_char(stream, desplazamiento);
	registros->ecx = deserializar_char(stream, desplazamiento);
	registros->edx = deserializar_char(stream, desplazamiento);
	registros->rax = deserializar_char(stream, desplazamiento);
	registros->rbx = deserializar_char(stream, desplazamiento);
	registros->rcx = deserializar_char(stream, desplazamiento);
	registros->rdx = deserializar_char(stream, desplazamiento); -> mal xq ax es una array de char, al cual no se le puede cambiar el puntero
	en ningun momento de la ejecucion, ni el tamaño*/


	/*memcpy(&tamanio, stream + *desplazamiento, sizeof(int));
	*desplazamiento += sizeof(int);
	memcpy(registros->bx, stream + *desplazamiento, tamanio);
	*desplazamiento += tamanio;

	memcpy(&tamanio, stream + *desplazamiento, sizeof(int));
	*desplazamiento += sizeof(int);
	memcpy(registros->cx, stream + *desplazamiento, tamanio);
	*desplazamiento += tamanio;

	memcpy(&tamanio, stream + *desplazamiento, sizeof(int));
	*desplazamiento += sizeof(int);
	memcpy(registros->dx, stream + *desplazamiento, tamanio);
	*desplazamiento += tamanio;

	memcpy(&tamanio, stream + *desplazamiento, sizeof(int));
	*desplazamiento += sizeof(int);
	memcpy(registros->eax, stream + *desplazamiento, tamanio);
	*desplazamiento += tamanio;

	memcpy(&tamanio, stream + *desplazamiento, sizeof(int));
	*desplazamiento += sizeof(int);
	memcpy(registros->ebx, stream + *desplazamiento, tamanio);
	*desplazamiento += tamanio;

	memcpy(&tamanio, stream + *desplazamiento, sizeof(int));
	*desplazamiento += sizeof(int);
	memcpy(registros->ecx, stream + *desplazamiento, tamanio);
	*desplazamiento += tamanio;

	memcpy(&tamanio, stream + *desplazamiento, sizeof(int));
	*desplazamiento += sizeof(int);
	memcpy(registros->edx, stream + *desplazamiento, tamanio);
	*desplazamiento += tamanio;

	memcpy(&tamanio, stream + *desplazamiento, sizeof(int));
	*desplazamiento += sizeof(int);
	memcpy(registros->rax, stream + *desplazamiento, tamanio);
	*desplazamiento += tamanio;

	memcpy(&tamanio, stream + *desplazamiento, sizeof(int));
	*desplazamiento += sizeof(int);
	memcpy(registros->rbx, stream + *desplazamiento, tamanio);
	*desplazamiento += tamanio;

	memcpy(&tamanio, stream + *desplazamiento, sizeof(int));
	*desplazamiento += sizeof(int);
	memcpy(registros->rcx, stream + *desplazamiento, tamanio);
	*desplazamiento += tamanio;

	memcpy(&tamanio, stream + *desplazamiento, sizeof(int));
	*desplazamiento += sizeof(int);
	memcpy(registros->rdx, stream + *desplazamiento, tamanio);
	*desplazamiento += tamanio;*/

	return registros;
}

t_list* deserializar_tabla_segmentos(void* stream, int* desplazamiento, int cant_segmentos){
	t_list* tabla_segmentos= list_create();
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

 char* deserializar_char(void* stream, int* desplazamiento){
	 int tamanio;
	 char* registro;

	 memcpy(&tamanio, stream + *desplazamiento, sizeof(int));
	 *desplazamiento += sizeof(int);
	 registro = stream + *desplazamiento;
	 *desplazamiento+=tamanio;
	 return registro;
	 /*->si hacemos lo descomentado habria que usar strncpy, en vez de strcpy
	 char registro[tamanio];
	 char* ptr_registro = registro;

	 memcpy(ptr_registro, stream + *desplazamiento, tamanio);
	 *desplazamiento += tamanio;

	 return ptr_registro;*/
 }

uint32_t deserializar_uint32(void* stream, int* desplazamiento){
	uint32_t* variable;
	uint32_t tamanio;
	//uint32_t valor;

	memcpy(&tamanio, stream + *desplazamiento, sizeof(uint32_t));
	*desplazamiento+= sizeof(uint32_t);
	variable = stream + *desplazamiento;
	*desplazamiento+= tamanio;
	/*variable = malloc(tamanio);
	memcpy(variable,stream + *desplazamiento, tamanio);   -> la funcion estaria deserializando 2 uint32, por el primer tamaño
	*desplazamiento+= tamanio;                            -> haria una funcion agregar_uint32_a_paquete() para no hacer lo anterior

	valor=*variable;
	free(variable);
	return valor;*/
	return *variable;

	/* hacer que el puntero apunte a la misma direccion donde el buffer tiene ese valor*/
}

t_list* deserializar_instrucciones(void* stream, uint32_t cant_instrucciones, int* desplazamiento){
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
