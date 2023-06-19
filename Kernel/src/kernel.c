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

    pthread_create(&planificador_corto_plazo, NULL, (void*) planificarCortoPlazo, NULL);
    pthread_detach(planificador_corto_plazo);

    while(1){
        int conexion_consola = esperar_cliente(server_kernel, kernel_logger);
        pthread_create(&atender_consolas, NULL, (void*) recibirProcesos, (void*) &conexion_consola);
        pthread_detach(atender_consolas);
    }

    liberar_conexion(server_kernel, kernel_logger);

	return EXIT_SUCCESS;
}

void* conectarFileSystem(){
	socket_fileSystem = crear_conexion(ip_filesystem, puerto_filesystem, kernel_logger, "File System");
	handshake(socket_fileSystem, 1, kernel_logger, "File System");

	enviar_mensaje("Conectado al Kernel", socket_fileSystem);
	return "";
}

void* conectarCPU(){
	socket_cpu = crear_conexion(ip_cpu, puerto_cpu, kernel_logger, "CPU");
	handshake(socket_cpu, 1, kernel_logger, "CPU");

	enviar_mensaje("Conectado al Kernel", socket_cpu);
	return "";
}


void* conectarMemoria(){
	socket_memoria = crear_conexion(ip_memoria, puerto_memoria, kernel_logger, "Memoria");
	handshake(socket_memoria, 1, kernel_logger, "Memoria");

	enviar_mensaje("Conectado al Kernel", socket_memoria);
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
		log_info(kernel_logger, "Se crea el proceso %d en NEW", nuevo_pcb->pid);
		ingresar_en_lista(nuevo_pcb, lista_new, "NEW", &semaforo_new, &cantidad_procesos_new, NEW);
		break;
	}
	return "";
}


void init_estructuras_planificacion(){
    lista_new = list_create();
    lista_ready = list_create();
    lista_recursos = list_create();

    lista_archivos_abiertos = list_create();

    devolver_ejecucion = 0;
    //recibi_instruccion = 0;

    int i = 0;
	char** ptr = recursos;
	for (char* c = *ptr; c; c=*++ptr) {
		char* str = string_new();
		strcat(str, c);
		uint32_t cantidad = atoi(instancias_recursos[i]);
		t_recurso* recurso = crear_recurso(str,cantidad);
		list_add(lista_recursos, recurso);
		i++;
	}

    hora_inicio = (long long)tiempo.tv_sec * 1000 + (long long)tiempo.tv_usec / 1000;
    sem_init(&semaforo_multiprogramacion, 0, grado_maximo_multiprogramacion);

    sem_init(&cantidad_procesos_new, 0, 0);
    sem_init(&cantidad_procesos_ready, 0, 0);

    pthread_mutex_init(&semaforo_new, NULL);
    pthread_mutex_init(&semaforo_ready, NULL);
    //pthread_mutex_init(&semaforo_execute, NULL);

}

void planificarLargoPlazo(){
	while(1){
		//el sem_post hay que hacerlo cuando un proceso termina
		sem_wait(&semaforo_multiprogramacion);
		sem_wait(&cantidad_procesos_new);

		pthread_mutex_lock(&semaforo_new);
		t_pcb* pcb = list_remove(lista_new, 0);
		pthread_mutex_unlock(&semaforo_new);

		char mensaje[30] = "INICIAR ";
		char numero[10];
		sprintf(numero, "%d", pcb->pid);
		strcat(mensaje, numero);
		enviar_mensaje(mensaje, socket_memoria);
		recibir_mensaje_memoria();

		pcb->tabla_segmentos = tablaNueva;
		pcb->tabla_segmentos->segmentos = tablaNueva->segmentos;

		struct timeval tiempo;
		gettimeofday(&tiempo, NULL);
		pcb->tiempo_ready = tiempo.tv_sec * 1000 + tiempo.tv_usec / 1000;
		//pcb->tiempo_ready = tiempo.tv_sec * 1000000 + tiempo.tv_usec;
		//pcb->tiempo_ready = tiempo.tv_sec;

		//mandar a memoria el proceso para iniciar estructuras
		ingresar_en_lista(pcb, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);
	}
}

void recibir_mensaje_memoria(){
	int cod_op;
	cod_op = recibir_operacion(socket_memoria);

	switch (cod_op) {
		case MENSAJE:
			char* recibi = recibir_instruccion(socket_memoria, kernel_logger);
			ejecutar_motivo_memoria(recibi);
			break;
		case PAQUETE:
			int size;
			void* buffer;
			int* tam_recibido = malloc(sizeof(int));
			*tam_recibido = 0;
			buffer = recibir_buffer(&size, socket_memoria);

			tablaNueva = deserializar_segmentos(buffer, tam_recibido);
			//printf("SASASASASAS\n\n\n");
			//imprimir_segmentos(tablaNueva);
			log_info(kernel_logger, "Recibi Tabla de Segmentos - PID: %d", tablaNueva->pid);

			*tam_recibido+=2*sizeof(int);
			send(socket_memoria, tam_recibido, sizeof(int), 0);
		default: break;
	}
}


void ejecutar_motivo_memoria(char* motivo){

	char** parametros = string_split(motivo, " ");
	char** instruccion = string_split(ultima_instruccion, " ");
	codigo_instruccion cod_instruccion = obtener_codigo_instruccion(parametros[0]);

	switch(cod_instruccion) {
	case OUT:
		log_cambiar_estado(pcb_a_ejecutar->pid, pcb_a_ejecutar->estado, EXITT);
		pcb_a_ejecutar->estado = EXITT;
		sem_post(&semaforo_multiprogramacion);
		log_info(kernel_logger, "Finaliza el proceso PID: %d - Motivo: OUT OF MEMORY ", pcb_a_ejecutar->pid);
		enviar_mensaje("-1", pcb_a_ejecutar->pid);
		liberar_conexion(pcb_a_ejecutar->pid, kernel_logger);
		devolver_ejecucion = 0;
		break;
	case SEGMENT:
		parametros = string_split(motivo, " ");
		instruccion = string_split(ultima_instruccion, " ");
		t_segmento* segmento_nuevo = malloc(sizeof(t_segmento));

		uintptr_t dir = strtoull(parametros[1], NULL, 16);
		void* base = (void*)dir;

		segmento_nuevo->direccion_base = base;
		segmento_nuevo->limite = base + atoi(instruccion[2]);
		list_add_in_index(pcb_a_ejecutar->tabla_segmentos->segmentos, atoi(instruccion[1]), segmento_nuevo);

		ingresar_en_lista(pcb_a_ejecutar, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);
		devolver_ejecucion = 1;
		pcb_ejecutando = pcb_a_ejecutar;

		break;
	case COMPACT:
		break;
	default:
		break;

	}
}


void planificarCortoPlazo(){
	//sleep(10);
	while(1){
		sem_wait(&cantidad_procesos_ready);
		pthread_mutex_lock(&semaforo_ready);
		if (devolver_ejecucion == 1){//para las instrucciones que luego de realizarlas, sigue el mismo proceso(como SIGNAL)
			list_remove_element(lista_ready, pcb_ejecutando);
			pcb_a_ejecutar = pcb_ejecutando;
			devolver_ejecucion = 0;
		}else{
			if(!(strcmp(algoritmo_planificacion, "FIFO"))){
				pcb_a_ejecutar = list_remove(lista_ready, 0);// si es FIFO saca el primero de ready
			}else {
				pcb_a_ejecutar = pcb_elegido_HRRN();
			}
		}
		pthread_mutex_unlock(&semaforo_ready);
		log_cambiar_estado(pcb_a_ejecutar->pid, pcb_a_ejecutar->estado, EXEC);
		pcb_a_ejecutar->estado = EXEC;

		enviar_pcb(pcb_a_ejecutar);
		recibir_mensaje_cpu();
	}
}

t_pcb* pcb_elegido_HRRN(){
	int tcb_index = 0;
	float ratio_mayor = 0.0;
	t_pcb* pcb = malloc(sizeof(t_pcb));
	struct timeval hora_actual;
	for (int i = 0; i < list_size(lista_ready); i++) {
		t_pcb* pcb = list_get(lista_ready, i);
		gettimeofday(&hora_actual, NULL);
		int tiempo = (hora_actual.tv_sec * 1000 + hora_actual.tv_usec / 1000) - pcb->tiempo_ready;
		//int tiempo = (hora_actual.tv_sec * 1000000 + hora_actual.tv_usec) - pcb->tiempo_ready;
		//int tiempo = (hora_actual.tv_sec + hora_actual.tv_sec) - pcb->tiempo_ready;

		float ratio = ((float) (pcb->estimado_rafaga + tiempo)) / (float) pcb->estimado_rafaga;
		if (ratio > ratio_mayor){
			ratio_mayor = ratio;
			tcb_index = i;
		}

		//printf("\nPCB %d", pcb->pid);
		//printf("\nTiempo %d", tiempo);
		//printf("\nEstimado %d", pcb->estimado_rafaga);
		//printf("\nRatio %f\n", ratio);

	}
	pcb = list_remove(lista_ready, tcb_index);
	return pcb;
}

void enviar_pcb(t_pcb* pcb){
	t_paquete* paquete;
	paquete = serializar_pcb(pcb);
	int tamanio_pcb;
	memcpy(&tamanio_pcb, paquete->buffer->stream, sizeof(int));
	enviar_paquete(paquete, socket_cpu, kernel_logger, "cpu");
}

void recibir_mensaje_cpu(){
	int cod_op;
	cod_op = recibir_operacion(socket_cpu);
	switch (cod_op) {
		case MENSAJE:
			/*
			char mensaje = recibir_instruccion(socket_cpu, kernel_logger));
			ejecutar_segun_motivo(mensaje);
			recibi_instruccion = 1;*///nunca enviamos mensaje por que siempre recibimos el contexto de ejecucion
			break;
		case PAQUETE:
			int size;
			void* buffer;
			char* motivo;
			int* tam_recibido= malloc(sizeof(int));
			buffer = recibir_buffer(&size, socket_cpu);

			deserializar_contexto(buffer,tam_recibido, pcb_a_ejecutar);
			motivo = deserializar_motivo(buffer, tam_recibido);
			log_trace(kernel_logger, "Recibi contexto de ejecucion - PID: %d", pcb_a_ejecutar->pid);
			ejecutar_segun_motivo(motivo);
			recibi_instruccion=0;

			*tam_recibido+=2*sizeof(int);
			send(socket_cpu, tam_recibido, sizeof(int), 0);

	}
}

void ejecutar_segun_motivo(char* motivo){

	char** parametros = string_split(motivo, " ");
	codigo_instruccion cod_instruccion = obtener_codigo_instruccion(parametros[0]);

	switch(cod_instruccion) {

	case WAIT:
		estimar_rafaga(pcb_a_ejecutar);
		char** parametros = string_split(motivo, " ");
		existeRecurso = recurso_existe(parametros[1]);
		if (existeRecurso == -1){
			log_error(kernel_logger, "Finaliza el proceso PID: %d - Motivo: WAIT - %s ", pcb_a_ejecutar->pid, parametros[1]);
			log_cambiar_estado(pcb_a_ejecutar->pid, pcb_a_ejecutar->estado, EXITT);
			pcb_a_ejecutar->estado = EXITT;
			sem_post(&semaforo_multiprogramacion);
			enviar_mensaje("-1", pcb_a_ejecutar->pid);
			liberar_conexion(pcb_a_ejecutar->pid, kernel_logger);
		} else {
			descontar_recurso(list_get(lista_recursos, existeRecurso), pcb_a_ejecutar, kernel_logger);//vuelve a poner al proceso en ready
			//imprimir_recurso(list_get(lista_recursos, existeRecurso));
		}
		break;

	case YIELD:
		estimar_rafaga(pcb_a_ejecutar);
		ingresar_en_lista(pcb_a_ejecutar, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);
		break;
	case EXIT:
		log_cambiar_estado(pcb_a_ejecutar->pid, pcb_a_ejecutar->estado, EXITT);
		pcb_a_ejecutar->estado = EXITT;
		sem_post(&semaforo_multiprogramacion);
		log_info(kernel_logger, "Finaliza el proceso PID: %d - Motivo: SUCCESS ", pcb_a_ejecutar->pid);
		enviar_mensaje("-1", pcb_a_ejecutar->pid);
		liberar_conexion(pcb_a_ejecutar->pid, kernel_logger);
		break;
	case SIGNAL:
		//estimar_rafaga(pcb_a_ejecutar);-> no haria falta estimar rafaga por que sigue el mismo proceso
		parametros = string_split(motivo, " ");
		existeRecurso = recurso_existe(parametros[1]);

		if (existeRecurso == -1){
			log_error(kernel_logger, "Finaliza el proceso PID: %d - Motivo: SIGNAL - %s ", pcb_a_ejecutar->pid, parametros[1]);
			pcb_a_ejecutar->estado = EXITT;
			sem_post(&semaforo_multiprogramacion);
			enviar_mensaje("-1", pcb_a_ejecutar->pid);
			liberar_conexion(pcb_a_ejecutar->pid, kernel_logger);
		} else {
			ingresar_en_lista(pcb_a_ejecutar, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);
			devolver_ejecucion = 1;
			pcb_ejecutando = pcb_a_ejecutar;
			sumar_recurso(list_get(lista_recursos, existeRecurso), pcb_ejecutando->pid, kernel_logger);
		}
		break;

	case I_O:
		estimar_rafaga(pcb_a_ejecutar);
		parametros = string_split(motivo, " ");

		log_info(kernel_logger, "PID: %d - Ejecuta IO: %s", pcb_a_ejecutar->pid, parametros[1]);
		log_cambiar_estado(pcb_a_ejecutar->pid, pcb_a_ejecutar->estado, BLOCKED);
		log_info(kernel_logger, "PID: %d - Bloqueado por: %s", pcb_a_ejecutar->pid, "IO");
		pcb_a_ejecutar->estado = BLOCKED;

		int tiempo = atoi(parametros[1]);

		t_thread_args* argumentos_hilo = malloc(sizeof(t_thread_args));
		argumentos_hilo->pcb = pcb_a_ejecutar;
		argumentos_hilo->duracion = tiempo;

		pthread_t io_procesos;
		pthread_create(&io_procesos, NULL ,(void*) ejecutar_io, argumentos_hilo);
		pthread_detach(io_procesos);
		break;

	case F_OPEN:
		parametros = string_split(motivo, " ");
		log_info(kernel_logger, "PID: %d - Abrir Archivo: %s", pcb_a_ejecutar->pid, parametros[1]);

		t_archivo* archivo_abrir = buscar_archivo_abierto(parametros[1]);


		if(archivo_abrir != NULL){
			printf("ABIERTO PERRI");
		} else{
			enviar_mensaje(motivo, socket_fileSystem);
			char* mensaje = recibir_mensaje_filesystem();
			printf("CERRADO PERRI");
		}

		ingresar_en_lista(pcb_a_ejecutar, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);
		devolver_ejecucion = 1;
		pcb_ejecutando = pcb_a_ejecutar;
		break;

	case F_CLOSE:
		parametros = string_split(motivo, " ");
		log_info(kernel_logger, "PID: %d - Cerrar Archivo: %s", pcb_a_ejecutar->pid, parametros[1]);
		enviar_mensaje(motivo, socket_fileSystem);
		ingresar_en_lista(pcb_a_ejecutar, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);
		devolver_ejecucion = 1;
		pcb_ejecutando = pcb_a_ejecutar;
		break;

	case F_SEEK:
		parametros = string_split(motivo, " ");
		log_info(kernel_logger, "PID: %d - Actualizar puntero Archivo: %s -> %s", pcb_a_ejecutar->pid, parametros[1], parametros[2]);
		enviar_mensaje(motivo, socket_fileSystem);
		ingresar_en_lista(pcb_a_ejecutar, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);
		devolver_ejecucion = 1;
		pcb_ejecutando = pcb_a_ejecutar;
		break;

	case F_READ:
		parametros = string_split(motivo, " ");
		log_info(kernel_logger, "PID: %d - Leer Archivo: %s "
				"- Puntero: %s "
				"- Direccion Memoria: %s "
				"- Tamaño: %s", pcb_a_ejecutar->pid, parametros[1], parametros[2], parametros[3], parametros[4]);
		enviar_mensaje(motivo, socket_fileSystem);
		ingresar_en_lista(pcb_a_ejecutar, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);
		devolver_ejecucion = 1;
		pcb_ejecutando = pcb_a_ejecutar;
		break;

	case F_WRITE:
		parametros = string_split(motivo, " ");
		log_info(kernel_logger, "PID: %d - Escribir Archivo: %s "
				"- Puntero: %s "
				"- Direccion Memoria: %s "
				"- Tamaño: %s", pcb_a_ejecutar->pid, parametros[1], parametros[2], parametros[3], parametros[4]);
		enviar_mensaje(motivo, socket_fileSystem);
		ingresar_en_lista(pcb_a_ejecutar, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);
		devolver_ejecucion = 1;
		pcb_ejecutando = pcb_a_ejecutar;
		break;

	case F_TRUNCATE:
		parametros = string_split(motivo, " ");
		log_info(kernel_logger, "PID: %d - Truncar Archivo: %s - Tamaño: %s", pcb_a_ejecutar->pid, parametros[1], parametros[2]);
		enviar_mensaje(motivo, socket_fileSystem);
		ingresar_en_lista(pcb_a_ejecutar, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);
		devolver_ejecucion = 1;
		pcb_ejecutando = pcb_a_ejecutar;
		break;

	case CREATE_SEGMENT:

		parametros = string_split(motivo, " ");
		log_info(kernel_logger, "PID: %d - Crear Segmento - ID: %s - Tamaño: %s", pcb_a_ejecutar->pid, parametros[1], parametros[2]);

		char numero[4];
		sprintf(numero, "%d", pcb_a_ejecutar->pid);
		strcat(motivo, " ");
		strcat(motivo, numero);
		strcat(ultima_instruccion, motivo);

		enviar_mensaje(motivo, socket_memoria);
		recibir_mensaje_memoria();

		//imprimir_segmentos(pcb_a_ejecutar->tabla_segmentos);
		//ingresar_en_lista(pcb_a_ejecutar, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);
		//devolver_ejecucion = 1;
		//pcb_ejecutando = pcb_a_ejecutar;
		break;

	case DELETE_SEGMENT:
		parametros = string_split(motivo, " ");
		log_info(kernel_logger, "PID: %d - Crear Segmento - ID: %s - Tamaño: %s", pcb_a_ejecutar->pid, parametros[1], parametros[2]);
		enviar_mensaje(motivo, socket_memoria);
		ingresar_en_lista(pcb_a_ejecutar, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);
		devolver_ejecucion = 1;
		pcb_ejecutando = pcb_a_ejecutar;
		break;

	case MOV_IN: //CPU solo comunica a Kernel cuando hay SEG_FAULT
		pcb_a_ejecutar->estado = EXITT;
		sem_post(&semaforo_multiprogramacion);
		log_info(kernel_logger, "Finaliza el proceso PID: %d - Motivo: SEG_FAULT", pcb_a_ejecutar->pid);
		enviar_mensaje("-1", pcb_a_ejecutar->pid);
		liberar_conexion(pcb_a_ejecutar->pid, kernel_logger);
		break;

	case MOV_OUT: //CPU solo comunica a Kernel cuando hay SEG_FAULT
		pcb_a_ejecutar->estado = EXITT;
		sem_post(&semaforo_multiprogramacion);
		log_info(kernel_logger, "Finaliza el proceso PID: %d - Motivo: SEG_FAULT", pcb_a_ejecutar->pid);
		enviar_mensaje("-1", pcb_a_ejecutar->pid);
		liberar_conexion(pcb_a_ejecutar->pid, kernel_logger);
		break;

	default:
		break;
	}
}

char* recibir_mensaje_filesystem(){
	int cod_op;
	cod_op = recibir_operacion(socket_fileSystem);
	char mensaje[30] = "";
	strcat(mensaje, recibir_instruccion(socket_fileSystem, kernel_logger));
	return mensaje;
	/*switch (cod_op) {
		case MENSAJE:
			char mensaje[30] = "";
			strcat(mensaje, recibir_instruccion(socket_fileSystem, kernel_logger));
			break;
	}*/
}

void ejecutar_io(t_thread_args* args){
	sleep(args->duracion);
	struct timeval tiempo;
	gettimeofday(&tiempo, NULL);
	args->pcb->tiempo_ready = tiempo.tv_sec * 1000 + tiempo.tv_usec / 1000; // actualiza la llegada a ready del proceso
	ingresar_en_lista(args->pcb, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);
	free(args);
	pthread_exit(0);
}

void estimar_rafaga(t_pcb* pcb){
	uint32_t tiempo_viejo = pcb->tiempo_ready;
	uint32_t estimado_viejo = pcb->estimado_rafaga;
	struct timeval tiempo;
	gettimeofday(&tiempo, NULL);
	pcb->tiempo_ready = tiempo.tv_sec * 1000 + tiempo.tv_usec / 1000; // milisec
	//pcb->tiempo_ready = tiempo.tv_sec * 1000000 + tiempo.tv_usec; //micro
	//pcb->tiempo_ready = tiempo.tv_sec; //seg
	float alpha = 1 - hrrn_alfa;
	pcb->estimado_rafaga = (alpha * estimado_viejo + hrrn_alfa * (pcb->tiempo_ready - tiempo_viejo));
}


void cerrar_conexiones(){
	log_trace(kernel_logger ,"Cerrando Conexiones");
	close(server_kernel);
	close(socket_cpu);
	close(socket_memoria);
	close(socket_fileSystem);
	exit(1);
}

void imprimirSemaforos(){
	int semaphoreValue;
	printf("\n\n\n\nElementos %d ",list_size(lista_ready));
	sem_getvalue(&semaforo_multiprogramacion, &semaphoreValue);
	printf("Semaforo Multi %d \n\n\n\n", semaphoreValue);
	sem_getvalue(&cantidad_procesos_ready, &semaphoreValue);
	printf("Semaforo Ready %d \n\n\n\n", semaphoreValue);
}

