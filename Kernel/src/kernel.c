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

	server_kernel = iniciar_servidor(NULL, puerto_escucha, kernel_logger);
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
    	//usleep(1000);
    	int conexion_consola = esperar_cliente(server_kernel, kernel_logger);
    	//printf("\n consola: %d\n", conexion_consola);
    	int *conexion_ptr = malloc(sizeof(int));
    	*conexion_ptr = conexion_consola;
    	pthread_create(&atender_consolas, NULL, (void*) recibirProcesos, (void*) conexion_ptr);
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
	uint32_t respuesta;
	//printf("\nconexion: %d, hilo: %d\n", conexion, process_get_thread_id());
	//pthread_mutex_lock(&sem_procesos);
	recv(conexion, &respuesta, sizeof(uint32_t), MSG_WAITALL);
	//printf("\n consola: %d, hilo: %d respuesta: %d\n", conexion, process_get_thread_id(),respuesta);
	if(respuesta == 1){
	   send(conexion, &resultOk, sizeof(uint32_t), 0);
	}
	else{
	   send(conexion, &resultError, sizeof(uint32_t), 0);
	}

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
	//pthread_mutex_unlock(&sem_procesos);
	return "";
}


void init_estructuras_planificacion(){
    lista_new = list_create();
    lista_ready = list_create();
    lista_recursos = list_create();
    lista_archivos_abiertos = list_create();
    lista_pcbs = list_create();

    devolver_ejecucion = 0;
    fileSystem_ejecutando =0;

    int i = 0;
	char** ptr = recursos;
	for (char* c = *ptr; c; c=*++ptr) {
		char* str = string_new();
		string_append(&str, c);
		uint32_t cantidad = atoi(instancias_recursos[i]);
		t_recurso* recurso = crear_recurso(str,cantidad);
		list_add(lista_recursos, recurso);
		i++;
		free(str);
	}

    hora_inicio = (long long)tiempo.tv_sec * 1000 + (long long)tiempo.tv_usec / 1000;
    //hora_inicio = (long long)tiempo.tv_sec * 1000000 + (long long)tiempo.tv_usec;
    //hora_inicio = (long long)tiempo.tv_sec + (long long)tiempo.tv_sec;
    sem_init(&semaforo_multiprogramacion, 0, grado_maximo_multiprogramacion);

    sem_init(&cantidad_procesos_new, 0, 0);
    sem_init(&cantidad_procesos_ready, 0, 0);

    pthread_mutex_init(&sem_fileSystem, NULL);
    pthread_mutex_init(&sem_memoria, NULL);

    pthread_mutex_init(&semaforo_new, NULL);
    pthread_mutex_init(&semaforo_ready, NULL);


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
		pthread_mutex_lock(&sem_memoria);
		enviar_mensaje(mensaje, socket_memoria);
		recibir_mensaje_memoria("");
		pthread_mutex_unlock(&sem_memoria);

		liberarTablaSegmentos(pcb);
		pcb->tabla_segmentos = tablaNueva;
		pcb->tabla_segmentos->segmentos = tablaNueva->segmentos;

		list_add(lista_pcbs,pcb);
		mandar_a_ready(pcb);
	}
}

void recibir_mensaje_memoria(char* inst){
	int cod_op;
	cod_op = recibir_operacion(socket_memoria);

	switch (cod_op) {
		case MENSAJE:
			char* recibi = recibir_instruccion(socket_memoria, kernel_logger);
			ejecutar_motivo_memoria(recibi, inst);
			free(recibi);
			break;
		case PAQUETE:
			int size;
			void* buffer;
			int tam_recibido = 0;
			buffer = recibir_buffer(&size, socket_memoria);

			tablaNueva = deserializar_segmentos(buffer, &tam_recibido);
			//printf("SASASASASAS\n\n\n");
			//imprimir_segmentos(tablaNueva);
			log_info(kernel_logger, "Recibi Tabla de Segmentos - PID: %d", tablaNueva->pid);

			tam_recibido+=2*sizeof(int);
			send(socket_memoria, &tam_recibido, sizeof(int), 0);
			free(buffer);
			break;
		default: break;
	}
}

t_pcb* buscar_pcb(t_list* lista, uint32_t pid_buscado){
	int elementos = list_size(lista);
	for (int i = 0; i < elementos; i++) {
		t_pcb* pcb = list_get(lista, i);
		if (pid_buscado == pcb->pid){
			return pcb;
		}
	}
	return NULL;
}

void ejecutar_motivo_memoria(char* motivo, char* inst){

	char** parametros = string_split(motivo, " ");
	char** instruccion = string_split(ultima_instruccion, " ");
	codigo_instruccion cod_instruccion = obtener_codigo_instruccion(parametros[0]);

	switch(cod_instruccion) {
	case OUT:
		log_cambiar_estado(pcb_a_ejecutar->pid, pcb_a_ejecutar->estado, EXITT);
		pcb_a_ejecutar->estado = EXITT;
		sem_post(&semaforo_multiprogramacion);
		log_error(kernel_logger, "Finaliza el proceso PID: %d - Motivo: OUT OF MEMORY", pcb_a_ejecutar->pid);
		enviar_mensaje("-1", pcb_a_ejecutar->pid);
		list_remove_element(lista_pcbs, pcb_a_ejecutar);
		liberar_recursos(pcb_a_ejecutar);
		liberar_archivos(pcb_a_ejecutar);
		liberar_conexion(pcb_a_ejecutar->pid, kernel_logger);
		liberar_pcb(pcb_a_ejecutar);
		//finalizar_proceso(pcb_a_ejecutar, "OUT OF MEMORY");
		devolver_ejecucion = 0;
		break;
	case SEGMENT:
		//parametros = string_split(motivo, " ");
		instruccion = string_split(ultima_instruccion, " ");
		memset(ultima_instruccion, 0, sizeof(ultima_instruccion));
		t_segmento* segmento_nuevo = list_get(pcb_a_ejecutar->tabla_segmentos->segmentos, atoi(instruccion[1]));

		uintptr_t dir = strtoull(parametros[1], NULL, 16);
		void* base = (void*)dir;

		segmento_nuevo->direccion_base = base;
		segmento_nuevo->limite = base + atoi(instruccion[2]);
		//imprimir_segmentos(pcb_a_ejecutar->tabla_segmentos);

		//ingresar_en_lista(pcb_a_ejecutar, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);
		devolver_ejecucion = 1;
		//pcb_ejecutando = pcb_a_ejecutar;
		break;
	case COMPACT:
		if(fileSystem_ejecutando){
			log_info(kernel_logger, "Compactacion: Esperando Fin de Operaciones de FS");
		}
		pthread_mutex_lock(&sem_fileSystem);
		log_info(kernel_logger, "Compactacion: Se solicito compactacion");

		enviar_mensaje("COMPACT", socket_memoria);
		int elementos = list_size(lista_pcbs);
		//printf("\n ELEMENTOS %i \n", elementos);
		for(int i = 0; i < elementos; i++){
			recibir_mensaje_memoria("");
			t_pcb* pcb_modificar = buscar_pcb(lista_pcbs, tablaNueva->pid);

			liberarTablaSegmentos(pcb_modificar);
			pcb_modificar->tabla_segmentos = tablaNueva;
			pcb_modificar->tabla_segmentos->segmentos = tablaNueva->segmentos;
		}
		log_info(kernel_logger, "Se finalizo el proceso de compactacion");

		enviar_mensaje(inst, socket_memoria);
		recibir_mensaje_memoria("");
		pthread_mutex_unlock(&sem_fileSystem);
		break;
	default:
		break;

	}
}


void planificarCortoPlazo(){
	//sleep(10);
	while(1){
		if (devolver_ejecucion == 1){//para las instrucciones que luego de realizarlas, sigue el mismo proceso(como SIGNAL)
			/*list_remove_element(lista_ready, pcb_ejecutando);
			pcb_a_ejecutar = pcb_ejecutando;*/
			devolver_ejecucion = 0;
		}else{
			sem_wait(&cantidad_procesos_ready);
			pthread_mutex_lock(&semaforo_ready);
			if(!(strcmp(algoritmo_planificacion, "FIFO"))){
				pcb_a_ejecutar = list_remove(lista_ready, 0);// si es FIFO saca el primero de ready
			}else {
				pcb_a_ejecutar = pcb_elegido_HRRN();
			}
			pthread_mutex_unlock(&semaforo_ready);
			log_cambiar_estado(pcb_a_ejecutar->pid, pcb_a_ejecutar->estado, EXEC);
			pcb_a_ejecutar->estado = EXEC;
		}

		pcb_a_ejecutar->llegada_cpu = tiempo_actual();
		enviar_pcb(pcb_a_ejecutar);
		recibir_mensaje_cpu();
	}
}

uint32_t tiempo_actual(){
	/*en milisegundos*/
	struct timeval hora_actual;
	gettimeofday(&hora_actual, NULL);
	uint32_t tiempo = (hora_actual.tv_sec * 1000 + hora_actual.tv_usec / 1000);
	return tiempo;
}

t_pcb* pcb_elegido_HRRN(){
	int tcb_index = 0;
	float ratio_mayor = 0.0;
	t_pcb* pcb; /*= malloc(sizeof(t_pcb));*/
	struct timeval hora_actual;
	gettimeofday(&hora_actual, NULL);
	for (int i = 0; i < list_size(lista_ready); i++) {
		/*t_pcb* */pcb = list_get(lista_ready, i);

		int tiempo = (hora_actual.tv_sec * 1000 + hora_actual.tv_usec / 1000) - pcb->tiempo_ready;
		//int tiempo = (hora_actual.tv_sec * 1000000 + hora_actual.tv_usec) - pcb->tiempo_ready;
		//int tiempo = (hora_actual.tv_sec + hora_actual.tv_sec) - pcb->tiempo_ready;

		float ratio = ((float) (pcb->estimado_rafaga + tiempo)) / (float) pcb->estimado_rafaga;
		if (ratio > ratio_mayor){
			ratio_mayor = ratio;
			tcb_index = i;
		}


		/*printf("\nPCB %d", pcb->pid);
		printf("\nTiempo %d", tiempo);
		printf("\nEstimado %d", pcb->estimado_rafaga);
		printf("\nRatio %f\n", ratio);*/

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
			break;
		case PAQUETE:
			int size;
			void* buffer;
			char* motivo;
			int tam_recibido= 0;
			buffer = recibir_buffer(&size, socket_cpu);

			liberar_contexto_kernel(pcb_a_ejecutar);//liberar el contexto del pcb, xq va a tener otro
			deserializar_contexto(buffer,&tam_recibido, pcb_a_ejecutar);
			motivo = deserializar_motivo(buffer, &tam_recibido);
			log_trace(kernel_logger, "Recibi contexto de ejecucion - PID: %d", pcb_a_ejecutar->pid);
			ejecutar_segun_motivo(motivo);
			recibi_instruccion=0;

			tam_recibido+=2*sizeof(int);
			send(socket_cpu, &tam_recibido, sizeof(int), 0);
			//free(tam_recibido);
			free(buffer);
			//free(motivo);
	}
}

void ejecutar_segun_motivo(char* motivo){

	char** parametros;
	parametros = string_split(motivo, " ");
	codigo_instruccion cod_instruccion = obtener_codigo_instruccion(parametros[0]);


	switch(cod_instruccion) {

	case WAIT:
		//estimar_rafaga(pcb_a_ejecutar);-> solo se estima si el proceso de bloquea
		char** parametros = string_split(motivo, " ");
		existeRecurso = recurso_existe(parametros[1]);
		if (existeRecurso == -1){
			log_error(kernel_logger, "Finaliza el proceso PID: %d - Motivo: WAIT - %s ", pcb_a_ejecutar->pid, parametros[1]);
			list_remove_element(lista_pcbs, pcb_a_ejecutar);
			finalizar_proceso(pcb_a_ejecutar, "INVALID_RESOURCE");
		} else {
			descontar_recurso(list_get(lista_recursos, existeRecurso), pcb_a_ejecutar, kernel_logger);//vuelve a poner al proceso en ready
			//imprimir_recurso(list_get(lista_recursos, existeRecurso));
		}
		break;

	case YIELD:
		estimar_rafaga(pcb_a_ejecutar);
		mandar_a_ready(pcb_a_ejecutar);
		//ingresar_en_lista(pcb_a_ejecutar, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);
		break;
	case EXIT:
		list_remove_element(lista_pcbs, pcb_a_ejecutar);
		finalizar_proceso(pcb_a_ejecutar, "SUCCESS");
		break;
	case SIGNAL:
		//estimar_rafaga(pcb_a_ejecutar);-> no haria falta estimar rafaga por que sigue el mismo proceso
		parametros = string_split(motivo, " ");
		existeRecurso = recurso_existe(parametros[1]);

		if (existeRecurso == -1){
			log_error(kernel_logger, "Finaliza el proceso PID: %d - Motivo: SIGNAL - %s ", pcb_a_ejecutar->pid, parametros[1]);
			list_remove_element(lista_pcbs, pcb_a_ejecutar);
			finalizar_proceso(pcb_a_ejecutar, "INVALID_RESOURCE");

		} else {
			//ingresar_en_lista(pcb_a_ejecutar, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);
			devolver_ejecucion = 1;
			//pcb_ejecutando = pcb_a_ejecutar;
			sumar_recurso(list_get(lista_recursos, existeRecurso), pcb_a_ejecutar, kernel_logger);
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
				estimar_rafaga(pcb_a_ejecutar);
				list_add(archivo_abrir->listaBloqueados, pcb_a_ejecutar);
				log_cambiar_estado(pcb_a_ejecutar->pid, pcb_a_ejecutar->estado, BLOCKED);
				log_info(kernel_logger, "PID: %d - Bloqueado por: %s", pcb_a_ejecutar->pid, archivo_abrir->nombre);
				pcb_a_ejecutar->estado = BLOCKED;
				agregar_archivo_al_proceso(archivo_abrir, pcb_a_ejecutar);//lo agrega con el puntero en 0
				//printf("el archivo: %s ya estaba abierto, el proceso: %d se bloquea", archivo_abrir->nombre,pcb_a_ejecutar->pid);
			} else{
				//se abre el archivo, si no existe, se crea. y se agrega a la tabla global de archivos abiertos y del proceso
				//IMPLEMENTAR
				pthread_mutex_lock(&sem_fileSystem);
				enviar_mensaje(motivo, socket_fileSystem);
				char* mensaje_fopen = recibir_mensaje_filesystem();
				if(string_equals_ignore_case(mensaje_fopen, "OK")){
					free(mensaje_fopen);
					//printf("\n estaba creado\n");
				}else{
					char f_create[100]="";
					sprintf(f_create, "F_CREATE %s", parametros[1]);
					enviar_mensaje(f_create, socket_fileSystem);
					char* mensaje_create = recibir_mensaje_filesystem();
					//printf("\nse creo\n");
					free(mensaje_create);
				}

				pthread_mutex_unlock(&sem_fileSystem);
				t_archivo* archivo_creado = crear_archivo(parametros[1]);
				list_add(lista_archivos_abiertos, archivo_creado);

				//printf("el archivo_creado->nombre: %s\n\n", archivo_creado->nombre);
				agregar_archivo_al_proceso(archivo_creado, pcb_a_ejecutar);

				//ingresar_en_lista(pcb_a_ejecutar, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);
				devolver_ejecucion = 1;
				//pcb_ejecutando = pcb_a_ejecutar;
			}
			break;

	case F_CLOSE:
		/*cierra el archivo en la tabla del proceso con el archivo abierto
		desbloquea al proceso que estaba en la lista de bloqueados
		si esa lista estaba vacia -> se saca de la tabla global de archivos abiertos*/
		parametros = string_split(motivo, " ");
		char* archivo_a_cerrar = parametros[1];
		cerrar_archivo(archivo_a_cerrar, pcb_a_ejecutar);
		//ingresar_en_lista(pcb_a_ejecutar, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);
		devolver_ejecucion = 1;
		//pcb_ejecutando = pcb_a_ejecutar;
		break;

	case F_SEEK:
		//cambia el puntero que esta en la tabla de archivos del proceso
		//IMPLEMENTAR
		parametros = string_split(motivo, " ");
		char* nombre_archivo = parametros[1];
		uint32_t puntero = atoi(parametros[2]);
		log_info(kernel_logger, "PID: %d - Actualizar puntero Archivo: %s -> %d", pcb_a_ejecutar->pid, nombre_archivo, puntero);

		t_archivo* archivo_seek = get_archivo_del_proceso(nombre_archivo, pcb_a_ejecutar);
		archivo_seek->puntero = puntero;

		//ingresar_en_lista(pcb_a_ejecutar, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);
		devolver_ejecucion = 1;
		//pcb_ejecutando = pcb_a_ejecutar;
		break;

	case F_READ:
		parametros = string_split(motivo, " ");
		char* nom_archivo = parametros[1];
		t_archivo* archivo_read = get_archivo_del_proceso(nom_archivo, pcb_a_ejecutar);
		log_info(kernel_logger, "PID: %d - Leer Archivo: %s "
				"- Puntero: %d "
				"- Direccion Memoria: %s "
				"- Tamaño: %s", pcb_a_ejecutar->pid, parametros[1], archivo_read->puntero, parametros[2], parametros[3]);

		char* puntero_read = string_itoa(archivo_read->puntero);
		string_append(&motivo, " ");
		string_append(&motivo, puntero_read);
		pthread_t p_read;
		t_args_fileSystem* args_read = malloc(sizeof(t_args_fileSystem));
		args_read->motivo = malloc(strlen(motivo)+1);
		args_read->pcb = pcb_a_ejecutar;
		strcpy(args_read->motivo, motivo);
		pthread_create(&p_read, NULL ,(void*) accionFileSystem, args_read);
		pthread_detach(p_read);

		break;

	case F_WRITE:
		parametros = string_split(motivo, " ");
		char* nombre_arch = parametros[1];
		t_archivo* archivo_write = get_archivo_del_proceso(nombre_arch, pcb_a_ejecutar);
		log_info(kernel_logger, "PID: %d - Escribir Archivo: %s "
				"- Puntero: %d "
				"- Direccion Memoria: %s "
				"- Tamaño: %s", pcb_a_ejecutar->pid, parametros[1], archivo_write->puntero, parametros[2], parametros[3]);

		char* puntero_write = string_itoa(archivo_write->puntero);
		string_append(&motivo, " ");
		string_append(&motivo, puntero_write);
		pthread_t p_write;
		t_args_fileSystem* args_write = malloc(sizeof(t_args_fileSystem));
		args_write->motivo = malloc(strlen(motivo)+1);
		args_write->pcb = pcb_a_ejecutar;
		strcpy(args_write->motivo, motivo);
		pthread_create(&p_write, NULL ,(void*) accionFileSystem, args_write);
		pthread_detach(p_write);
		break;

	case F_TRUNCATE:
		estimar_rafaga(pcb_a_ejecutar);
		parametros = string_split(motivo, " ");
		log_info(kernel_logger, "PID: %d - Truncar Archivo: %s - Tamaño: %s", pcb_a_ejecutar->pid, parametros[1], parametros[2]);
		pthread_t p_truncar;
		t_args_fileSystem* args = malloc(sizeof(t_args_fileSystem));
		args->motivo = malloc(strlen(motivo)+1);
		args->pcb = pcb_a_ejecutar;
		strcpy(args->motivo, motivo);
		pthread_create(&p_truncar, NULL ,(void*) accionFileSystem, args);
		pthread_detach(p_truncar);
		break;

	case CREATE_SEGMENT:

		parametros = string_split(motivo, " ");

		if(!strcmp(parametros[1],"OUT_OF_MEMORY")){
			log_error(kernel_logger, "Finaliza el proceso PID: %d - Motivo: SEGMENTO MAYOR AL PERMITIDO", pcb_a_ejecutar->pid);
			finalizar_proceso(pcb_a_ejecutar, "OUT_OF_MEMORY");
		}else{
		log_info(kernel_logger, "PID: %d - Crear Segmento - ID: %s - Tamaño: %s", pcb_a_ejecutar->pid, parametros[1], parametros[2]);

		char numero[4];
		sprintf(numero, "%d", pcb_a_ejecutar->pid);
		string_append(&motivo, " ");
		string_append(&motivo, numero);
		strcat(ultima_instruccion, motivo);

		pthread_mutex_lock(&sem_memoria);
		enviar_mensaje(motivo, socket_memoria);
		recibir_mensaje_memoria(motivo);
		pthread_mutex_unlock(&sem_memoria);
		}
		break;

	case DELETE_SEGMENT:
		parametros = string_split(motivo, " ");
		log_info(kernel_logger, "PID: %d - Eliminar Segmento - Id segmento: %s", pcb_a_ejecutar->pid, parametros[1]);

		char numero_str[4];
		sprintf(numero_str, "%d", pcb_a_ejecutar->pid);
		string_append(&motivo, " ");
		string_append(&motivo, numero_str);
		strcat(ultima_instruccion, motivo);

		memset(ultima_instruccion, 0, sizeof(ultima_instruccion));
		pthread_mutex_lock(&sem_memoria);
		enviar_mensaje(motivo, socket_memoria);
		recibir_mensaje_memoria("");
		pthread_mutex_unlock(&sem_memoria);
		liberarTablaSegmentos(pcb_a_ejecutar);
		pcb_a_ejecutar->tabla_segmentos = tablaNueva;
		pcb_a_ejecutar->tabla_segmentos->segmentos = tablaNueva->segmentos;
		//imprimir_segmentos(pcb_a_ejecutar->tabla_segmentos);

		//ingresar_en_lista(pcb_a_ejecutar, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);
		devolver_ejecucion = 1;
		//pcb_ejecutando = pcb_a_ejecutar;
		break;

	case MOV_IN: //CPU solo comunica a Kernel cuando hay SEG_FAULT
		finalizar_proceso(pcb_a_ejecutar, "SEG_FAULT");
		break;

	case MOV_OUT: //CPU solo comunica a Kernel cuando hay SEG_FAULT
		finalizar_proceso(pcb_a_ejecutar, "SEG_FAULT");
		break;

	default:
		break;
	}
	string_iterate_lines(parametros, (void*) free);
	free(parametros);
}

void finalizar_proceso(t_pcb* pcb, char* motivo){
	log_cambiar_estado(pcb_a_ejecutar->pid, pcb->estado, EXITT);
	pcb->estado = EXITT;
	sem_post(&semaforo_multiprogramacion);
	char mensaje_mem[30] = "FINALIZAR ";
	char numero_mem[4];
	sprintf(numero_mem, "%d", pcb_a_ejecutar->pid);
	strcat(mensaje_mem, numero_mem);
	pthread_mutex_lock(&sem_memoria);
	enviar_mensaje(mensaje_mem, socket_memoria);
	pthread_mutex_unlock(&sem_memoria);

	log_info(kernel_logger, "Finaliza el proceso PID: %d - Motivo: %s ", pcb->pid, motivo);
	enviar_mensaje("-1", pcb->pid);
	liberar_recursos(pcb);
	liberar_archivos(pcb);
	liberar_conexion(pcb->pid, kernel_logger);
	liberar_pcb(pcb);
}

void liberar_recursos(t_pcb* pcb){
	for(int i=0;i< list_size(pcb->recursos);i++){
		t_recurso* recurso = list_get(pcb->recursos, i);
		sumar_recurso(recurso, pcb, kernel_logger);
	}
}

void liberar_archivos(t_pcb* pcb){
	for(int i=0;i<list_size(pcb->tabla_archivos);i++){
		t_archivo* archivo = list_get(pcb->tabla_archivos, i);
		cerrar_archivo(archivo->nombre, pcb);
	}
}

void accionFileSystem(t_args_fileSystem* args){//para TRUNCATE, READ Y WRITE
	char** parametros = string_split(args->motivo, " ");
	//printf("\nEL MOTIVO ES: %s\n\n", args->motivo);
	pthread_mutex_lock(&sem_fileSystem);
	fileSystem_ejecutando = 1;
	enviar_mensaje(args->motivo, socket_fileSystem);
	log_cambiar_estado(args->pcb->pid, args->pcb->estado, BLOCKED);
	log_info(kernel_logger, "PID: %d - Bloqueado por: %s", args->pcb->pid, parametros[1]);
	args->pcb->estado = BLOCKED;
	char* mensaje = recibir_mensaje_filesystem();
	free(mensaje);
	pthread_mutex_unlock(&sem_fileSystem);
	fileSystem_ejecutando = 0;
	mandar_a_ready(args->pcb);

	free(args->motivo);
	free(args);
}

void cerrar_archivo(char* nombre_archivo, t_pcb* pcb){
	t_archivo* archivo = get_archivo_del_proceso(nombre_archivo, pcb);

	log_info(kernel_logger, "PID: %d - Cerrar Archivo: %s", pcb->pid, archivo->nombre);
	if(archivo==NULL){
		//printf("\nno se encontro el archivo en la tabla de archivos del pcb\n\n");
		return;
	}
	list_remove_element(pcb->tabla_archivos, archivo);
	archivo->puntero=0;
	if(list_size(archivo->listaBloqueados)==0){
		list_remove_element(lista_archivos_abiertos, archivo);//si no hay procesos bloqueados, se saca de la tabla global
		free(archivo->nombre);
		list_destroy(archivo->listaBloqueados);
		free(archivo);
	}else{
		t_pcb* pcb_a_desbloquear = list_get(archivo->listaBloqueados, 0);
		list_remove_element(archivo->listaBloqueados, pcb_a_desbloquear);
		mandar_a_ready(pcb_a_desbloquear);
		//printf("\nPCB: %d desbloquedo xq pcb: %d cerro el archivo: %s\n\n",pcb_a_desbloquear->pid, pcb->pid, archivo->nombre);
	}

}


void ejecutar_io(t_thread_args* args){
	sleep(args->duracion);
	/*struct timeval tiempo;
	gettimeofday(&tiempo, NULL);
	args->pcb->tiempo_ready = tiempo.tv_sec * 1000 + tiempo.tv_usec / 1000; // actualiza la llegada a ready del proceso
	ingresar_en_lista(args->pcb, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);*/
	mandar_a_ready(args->pcb);
	free(args);
	pthread_exit(0);
}

void mandar_a_ready(t_pcb* pcb){
	ingresar_en_lista(pcb, lista_ready, "READY", &semaforo_ready, &cantidad_procesos_ready, READY);
	actualizar_llegada_a_ready(pcb);
}

void actualizar_llegada_a_ready(t_pcb* pcb){
	struct timeval tiempo;
	gettimeofday(&tiempo, NULL);
	pcb->tiempo_ready = tiempo.tv_sec * 1000 + tiempo.tv_usec / 1000;
	//pcb->tiempo_ready = tiempo.tv_sec * 1000000 + tiempo.tv_usec;
	//pcb->tiempo_ready = tiempo.tv_sec + tiempo.tv_sec;
}

void estimar_rafaga(t_pcb* pcb){
	/*cada vez que el proceso se desaloja de la cpu*/
	//uint32_t tiempo_viejo = pcb->tiempo_ready;
	uint32_t estimado_viejo = pcb->estimado_rafaga;
	uint32_t tiempoActual = tiempo_actual();
	uint32_t tiempo_ejecucion = tiempoActual - pcb->llegada_cpu;
	//pcb->tiempo_ready = tiempo.tv_sec * 1000 + tiempo.tv_usec / 1000; // milisec
	//pcb->tiempo_ready = tiempo.tv_sec * 1000000 + tiempo.tv_usec; //micro
	//pcb->tiempo_ready = tiempo.tv_sec; //seg
	float alpha = 1 - hrrn_alfa;
	pcb->estimado_rafaga = (alpha * estimado_viejo + hrrn_alfa * tiempo_ejecucion);

	//printf("\n\nestimado_viejo: %d,  estimado actual: %d, tiempo que tardo en cpu: %d\n\n", estimado_viejo, pcb->estimado_rafaga, tiempo_ejecucion);
}

char* recibir_mensaje_filesystem(){
	//int cod_op;
	//cod_op = recibir_operacion(socket_fileSystem);
	recibir_operacion(socket_fileSystem);
	char* mensaje= string_new();
	string_append(&mensaje, recibir_instruccion(socket_fileSystem, kernel_logger));
	return mensaje;
}


void cerrar_conexiones(){
	log_trace(kernel_logger ,"Cerrando Conexiones");
	close(server_kernel);
	close(socket_cpu);
	close(socket_memoria);
	close(socket_fileSystem);
	exit(1);
}

void liberar_pcb(t_pcb* pcb){
	free(pcb->registros_cpu);
	list_destroy_and_destroy_elements(pcb->instrucciones, liberar_elemento_list);
	list_destroy_and_destroy_elements(pcb->recursos, liberar_elemento_list);
	list_destroy_and_destroy_elements(pcb->tabla_archivos, liberar_elemento_list);
	liberarTablaSegmentos(pcb);
	free(pcb);
}

void liberar_elemento_list(void* elemento){
	//free(elemento);
}


void imprimirSemaforos(){
	int semaphoreValue;
	//printf("\n\n\n\nElementos %d ",list_size(lista_ready));
	sem_getvalue(&semaforo_multiprogramacion, &semaphoreValue);
	//printf("Semaforo Multi %d \n\n\n\n", semaphoreValue);
	sem_getvalue(&cantidad_procesos_ready, &semaphoreValue);
	//printf("Semaforo Ready %d \n\n\n\n", semaphoreValue);
}

void liberarTablaSegmentos(t_pcb* pcb){
	list_destroy_and_destroy_elements(pcb->tabla_segmentos->segmentos, liberar_elemento_list);//->liberar de otra forma la tabla de segmentos
	free(pcb->tabla_segmentos);
}

void liberar_contexto_kernel(t_pcb* pcb){
	free(pcb->registros_cpu);
}
