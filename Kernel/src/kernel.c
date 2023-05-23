#include "kernel.h"


int main(void){

	//signal(SIGINT, cerrar_conexiones);

	kernel_logger = iniciar_logger("../../logs/logKernel.log", "Kernel");

	if (kernel_logger == NULL){
		exit(1);
	}

	kernel_config = iniciar_config("../../config/Kernel.config", "Kernel");

	if (kernel_config == NULL){
		exit(2);
	}

	leer_configs(kernel_config, kernel_logger);
	log_info(kernel_logger, "¡Kernel iniciado correctamente!");

	server_kernel = iniciar_servidor(IP_SERVER, puerto_escucha, kernel_logger);
	log_info(kernel_logger, "Servidor listo para recibir al cliente");

	init_estructuras_planificacion();

	pthread_create(&conexionFileSystem, NULL, conectarFileSystem, NULL);
	pthread_detach(conexionFileSystem);
	pthread_create(&conexionCPU, NULL, conectarCPU, NULL);
	pthread_detach(conexionCPU);
	pthread_create(&conexionMemoria, NULL, conectarMemoria, NULL);
	pthread_detach(conexionMemoria);


	pthread_create(&planificador_largo_plazo, NULL, (void*) planificarLargoPlazo, NULL);
    pthread_detach(planificador_largo_plazo);

    pthread_create(&planificador_corto_plazo, NULL, (void*) planificarCortoPlazoFIFO, NULL);
    pthread_detach(planificador_corto_plazo);

	//pthread_create(&atender_consolas, NULL, recibirProcesos, NULL);
	//pthread_join(atender_consolas, NULL);

    while(1){
        int conexion_consola = esperar_cliente(server_kernel, kernel_logger);
        pthread_create(&atender_consolas, NULL, (void*) recibirProcesos, (void*) &conexion_consola);
        pthread_detach(atender_consolas);
        printf("\n\n lista ready: %d \n\n", list_size(lista_ready));
    }

    close(server_kernel);

	return EXIT_SUCCESS;
}

void* conectarFileSystem(){
	socket_fileSystem = crear_conexion(ip_filesystem, puerto_filesystem, kernel_logger, "File System");
	handshake(socket_fileSystem, 1, kernel_logger, "File System");

	enviar_mensaje("Soy el Kernel", socket_fileSystem);
	return "";
}

void* conectarCPU(){
	socket_cpu = crear_conexion(ip_cpu, puerto_cpu, kernel_logger, "CPU");
	handshake(socket_cpu, 1, kernel_logger, "CPU");

	enviar_mensaje("Soy el Kernel", socket_cpu);
	return "";
}

void* conectarMemoria(){
	socket_memoria = crear_conexion(ip_memoria, puerto_memoria, kernel_logger, "Memoria");
	handshake(socket_memoria, 1, kernel_logger, "Memoria");

	enviar_mensaje("Soy el Kernel", socket_memoria);
	return "";
}

void* recibirProcesos(int* p_conexion) {
	int conexion = *p_conexion;
	uint32_t resultOk = 0;
	uint32_t resultError = -1;

	recv(conexion, &respuesta, sizeof(uint32_t), MSG_WAITALL);
	if(respuesta == 1)
	   send(conexion, &resultOk, sizeof(uint32_t), 0);
	else
	   send(conexion, &resultError, sizeof(uint32_t), 0);

	t_list* lista;
	int cod_op = recibir_operacion(conexion);
	switch (cod_op) {
	case MENSAJE:
		recibir_mensaje(conexion, kernel_logger);
		break;
	case PAQUETE:
		uint32_t tamanio;
		lista = recibir_instrucciones(conexion, &tamanio);
		log_info(kernel_logger, "Nuevo Proceso recibido con exito");
		t_pcb* nuevo_pcb = crear_pcb(conexion, lista, estimacion_inicial);
		ingresar_en_lista(nuevo_pcb, lista_new, "NEW", &semaforo_new, NEW);
		print_pcb(nuevo_pcb);
		sem_post(&cantidad_procesos_new);

		//t_pcb* pcb = list_get(lista_new, list_size(lista_new)-1); -> el pcb ya paso a ready, por lo q da segmentation fault

		break;
	}
	return "";
}


void init_estructuras_planificacion(){
    lista_new = list_create();
    lista_ready = list_create();

    sem_init(&semaforo_multiprogramacion, 0, grado_maximo_multiprogramacion);

    sem_init(&cantidad_procesos_new, 0, 0);
    sem_init(&cantidad_procesos_ready, 0, 0);

    pthread_mutex_init(&semaforo_new, NULL);
    pthread_mutex_init(&semaforo_ready, NULL);
    pthread_mutex_init(&semaforo_execute, NULL);

}

void planificarLargoPlazo(){
	while(1){
		//el sem_post hay que hacerlo cuando un proceso termina
		sem_wait(&semaforo_multiprogramacion);
		sem_wait(&cantidad_procesos_new);
		pthread_mutex_lock(&semaforo_new);
		t_pcb* pcb = list_remove(lista_new, 0);
		pthread_mutex_unlock(&semaforo_new);
		//mandar a memoria el proceso para iniciar estructuras
		ingresar_en_lista(pcb, lista_ready, "READY", &semaforo_ready, READY);
		sem_post(&cantidad_procesos_ready);
	}
}

void planificarCortoPlazoFIFO(){
	while(1){
		sem_wait(&cantidad_procesos_ready);
		pthread_mutex_lock(&semaforo_ready);
		t_pcb* pcb_a_ejecutar = list_remove(lista_ready, 0);
		pthread_mutex_unlock(&semaforo_ready);
		pthread_mutex_lock(&semaforo_execute);
		enviar_pcb(pcb_a_ejecutar);
		recibir_mensaje_cpu(pcb_a_ejecutar);
		//print_pcb(pcb_a_ejecutar);
		printf("AHIVAAAAAAAAA");
		//recibir_instruccion_cpu(socket_cpu, kernel_logger);
		pthread_mutex_unlock(&semaforo_execute);
	}
}

void enviar_pcb(t_pcb* pcb){
	t_paquete* paquete;
	paquete = serializar_pcb(pcb);


	int tamanio_pcb;
	memcpy(&tamanio_pcb, paquete->buffer->stream, sizeof(int));
	printf("\n pcb a ejecutar:\n\n");

	printf("\ntam_enviado: %ld\n", paquete->buffer->size + 2*sizeof(int));

	enviar_paquete(paquete, socket_cpu, kernel_logger, "kernel");
}

void recibir_mensaje_cpu(t_pcb* pcb){
	int cod_op;
	cod_op = recibir_operacion(socket_cpu);
	printf("\n cod_op: %d \n", cod_op);
	switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(socket_cpu, kernel_logger);
			break;
		case PAQUETE:
			int size;
			void* buffer;
			char* motivo;
			int* tam_recibido= malloc(sizeof(int));
			buffer = recibir_buffer(&size, socket_cpu);
			printf("\n recibi buffer \n");

			deserializar_contexto(buffer,tam_recibido, pcb);
			motivo = deserializar_motivo(buffer, tam_recibido);
			ejecutar_segun_motivo(motivo, pcb);
			//contexto_de_ejecucion = deserializar_pcb(buffer, tam_recibido);

			*tam_recibido+=2*sizeof(int);
			printf("\n tamanio recibido: %d\n", *tam_recibido);
			//printf("puntero: %p\n", tam_recibido);
			int var_send_ = send(socket_cpu, tam_recibido, sizeof(int), 0);
			//printf("var_send: %d\n", var_send_);

			printf("\n recibi contexto:\n");
			print_pcb(pcb);
		/*case INSTRUCCION:
			char* instruccion_de_cpu = recibir_instruccion_cpu(socket_cpu, kernel_logger);
			printf("%s", instruccion_de_cpu);
			break;*/
	}
}

void ejecutar_segun_motivo(char* motivo, t_pcb* pcb){
	if(strcmp(motivo, "YIELD")==0){
		//agregar al final de la lista ready
		ingresar_en_lista(pcb, lista_ready, "READY", &semaforo_ready, READY);
		sem_post(&cantidad_procesos_ready);
		printf("ejecutando yield");

	}else if(strcmp(motivo, "EXIT")==0){
		pcb->estado = EXITT;
		printf("ejecutando exit");
		liberar_conexion(pcb->pid);
	}
}



void cerrar_conexiones(){
	printf("\ncerrando conexiones\n");

	close(server_kernel);
	close(socket_cpu);
	close(socket_memoria);
	close(socket_fileSystem);
	printf("cerre conexiones");
	exit(1);
}


