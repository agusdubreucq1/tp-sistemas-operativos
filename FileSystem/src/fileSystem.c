#include "fileSystem.h"


int main(void){

	fileSystem_logger = iniciar_logger("../../logs/logFileSystem.log", "FileSystem");

	if (fileSystem_logger == NULL){
		exit(1);
	}

	fileSystem_config = iniciar_config("../../config/FileSystem.config", "FileSystem");

	if (fileSystem_config == NULL){
		exit(2);
	}

	leer_configs(fileSystem_config, fileSystem_logger);
	log_info(fileSystem_logger, "¡File System iniciado correctamente!");

	server_fileSystem = iniciar_servidor(NULL, puerto_escucha, fileSystem_logger);
	log_info(fileSystem_logger, "Servidor listo para recibir al cliente");

	inicializar_estructuras();

	pthread_create(&conexionMemoria, NULL, conectarMemoria, NULL);
	pthread_detach(conexionMemoria);
	pthread_create(&atender_kernel, NULL, abrirSocketKernel, NULL);
	pthread_join(atender_kernel, NULL);

	return EXIT_SUCCESS;
}

void* abrirSocketKernel(){
	socket_Kernel = esperar_cliente(server_fileSystem, fileSystem_logger);

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
		recibir_mensaje(socket_Kernel, fileSystem_logger);
		break;
	}

	while(1){
		recibir_mensaje_kernel();
	}

	return "";
}

void* conectarMemoria(){
	socket_memoria = crear_conexion(ip_memoria, puerto_memoria, fileSystem_logger, "Memoria");
	handshake(socket_memoria, 1, fileSystem_logger, "Memoria");

	enviar_mensaje("Conectado al FileSystem", socket_memoria);

	sem_post(&espera_cerrar);

	/*while(1){
		//recibir_mensaje_memoria();
	}*/

	return "";
}

void recibir_mensaje_kernel(){
	int cod_op;
	cod_op = recibir_operacion(socket_Kernel);
	switch (cod_op) {
		case MENSAJE:
			char* mensaje = string_new();
			char* instruccion = recibir_instruccion(socket_Kernel, fileSystem_logger);
			string_append(&mensaje, instruccion);
			ejecutar_instruccion(mensaje);
			free(mensaje);
			free(instruccion);
			break;
		default: break;
	}
}

void recibir_mensaje_memoria(){
	int cod_op;
	cod_op = recibir_operacion(socket_memoria);
	switch (cod_op) {
		case MENSAJE:
			char* mensaje = string_new();
			char* instruccion = recibir_instruccion(socket_memoria, fileSystem_logger);
			string_append(&mensaje,instruccion);
			//ejecutar
			free(mensaje);
			free(instruccion);
			break;
		default: break;
	}
}

void inicializar_estructuras(){

	levantar_superBloque(path_superbloque, fileSystem_logger);
	inicializar_bitmap();
	inicializar_bloques();
	inicializar_FCBs();

	//printf("todo creado\n");
}


int existe_archivo(char* nombre){
	for(int i=0; i<list_size(lista_fcb);i++){
		t_fcb* fcb= list_get(lista_fcb, i);
		if(string_equals_ignore_case(fcb->nombre_archivo, nombre)){
			return 1;
		}
	}
	return 0;
}

void crear_archivo(char* nombre){
	t_fcb* nuevo_fcb = crear_fcb(nombre);
	list_add(lista_fcb, nuevo_fcb);
}

void agregar_bloque(t_fcb* fcb, int* asignados){
	if((*asignados) ==0){
		fcb->puntero_directo = asignar_bloque();
	}else{
		if((*asignados)==1){
			fcb->puntero_indirecto = asignar_bloque();
		}
		uint32_t bloque = asignar_bloque();
		int posicion = fcb->puntero_indirecto * tamanio_bloque + ((*asignados)-1)*sizeof(uint32_t);
		escribirArchivoBloques((void*)&bloque, posicion, sizeof(uint32_t));
	}
	(*asignados)++;
}

void agrandar(t_fcb* fcb, int asignados, int necesarios){
	int agregar = necesarios - asignados;
		if(asignados == 0 && necesarios ==1){
			fcb->puntero_directo = asignar_bloque();
		}else{
			for(int i=0; i<agregar; i++){
				agregar_bloque(fcb, &asignados);
			}
			usleep(retardo_acceso_bloque*1000);
			log_info(fileSystem_logger, "Acceso Bloque - Archivo: %s - Bloque Archivo:%s - Bloque File System %d",fcb->nombre_archivo, "puntero", fcb->puntero_indirecto);
		}
}

void liberar_bloque(t_fcb* fcb, int* asignados){
	if((*asignados)==1){
		//printf("liberando PD: %d\n", fcb->puntero_directo);
		desasignar_bloque(fcb->puntero_directo);
	}else{
		int posicion = fcb->puntero_indirecto* tamanio_bloque + ((*asignados) - 2)*4;
		uint32_t bloque_a_liberar;
		leerArchivoBloques((void*)&bloque_a_liberar,posicion, sizeof(uint32_t));
		//printf("liberando bloque: %d\n", bloque_a_liberar);
		desasignar_bloque(bloque_a_liberar);
		if((*asignados)==2){
			//printf("\nlibero el puntero indirecto: %d \n", fcb->puntero_indirecto);
			desasignar_bloque(fcb->puntero_indirecto);//libero el bloque del puntero indirecto
			//imprimir_bitmap_20(bitmap);
		}
	}
	(*asignados)-=1;
}

void achicar(t_fcb* fcb, int asignados, int necesarios){
	int liberar =asignados - necesarios;
	//printf("\n bloques a liberar: %d\n", liberar);
	if(asignados==1 && necesarios==0){
		desasignar_bloque(fcb->puntero_directo);
	}else{
		usleep(retardo_acceso_bloque);
		log_info(fileSystem_logger,
			"Acceso Bloque - Archivo: %s - Bloque Archivo: %s - Bloque File System: %d", fcb->nombre_archivo, "punteros",fcb->puntero_indirecto );
		for(int i=0;i<liberar;i++){
			liberar_bloque(fcb, &asignados);
		}

	}
}

void cambiar_tamanio(char* archivo, int tamanio){
	t_fcb* fcb = fcb_segun_nombre(archivo);
	int asignados = bloques_necesarios(fcb->tamano_archivo);
	int necesarios = bloques_necesarios(tamanio);
	if(necesarios > asignados){
		agrandar(fcb, asignados, necesarios);
	}
	if(asignados > necesarios){
		achicar(fcb, asignados, necesarios);
	}
	fcb->tamano_archivo = tamanio;
	grabar_fcb(fcb);
	grabar_bitmap(bitmap);
	//imprimir_bitmap_20(bitmap);
}

int bloqueSegunPuntero(int puntero){
	return puntero/tamanio_bloque;
}

int offsetSegunPuntero(int puntero){
	return puntero % tamanio_bloque;
}

int bloqueLogicoAFisico(t_fcb* fcb, int num_bloque){
	int bloque_fisico;
	if(num_bloque == 0){
		return fcb->puntero_directo;
	}else{
		int posicion = fcb->puntero_indirecto * tamanio_bloque + (num_bloque - 1)*sizeof(uint32_t);
		leerArchivoBloques((void*)&bloque_fisico, posicion, sizeof(uint32_t));
		return bloque_fisico;
	}
}

int posicionArchivoBloques(int num_bloque, int offset){
	return num_bloque*tamanio_bloque + offset;
}

void escribir(char* archivo, void* aEscribir, int puntero, int tamanio){
	int tam_escrito = 0;
	log_trace(fileSystem_logger, "escribiendo archivo: %s", archivo);
	while(tam_escrito < tamanio){
		int tam_a_escribir_max = tamanio_bloque - (puntero % tamanio_bloque);
		int tam_a_escribir = min(tamanio - tam_escrito, tam_a_escribir_max);
		t_fcb* fcb = fcb_segun_nombre(archivo);
		int bloque_logico = bloqueSegunPuntero(puntero);
		int bloque_fisico = bloqueLogicoAFisico(fcb, bloque_logico);
		int offset = offsetSegunPuntero(puntero);
		int posicion = posicionArchivoBloques(bloque_fisico, offset);
		escribirArchivoBloques(aEscribir + tam_escrito, posicion, tam_a_escribir);

		usleep(retardo_acceso_bloque);
		log_info(fileSystem_logger, "Acceso Bloque - Archivo:%s - Bloque Archivo:%d - Bloque File System: %d", archivo, bloque_logico, bloque_fisico);

		tam_escrito += tam_a_escribir;
		puntero += tam_a_escribir;
	}

}

int min(int num1, int num2){
	if(num1 < num2){
		return num1;
	}else{
		return num2;
	}
}

void leer(char* archivo, void* aLeer, int puntero, int tamanio){
	int tam_leido = 0;
	log_trace(fileSystem_logger, "leyendo archivo: %s", archivo);
	while(tam_leido < tamanio){
		int tam_a_leer_max = tamanio_bloque - (puntero % tamanio_bloque);
		int tam_a_leer = min(tamanio-tam_leido, tam_a_leer_max);
		t_fcb* fcb = fcb_segun_nombre(archivo);
		int bloque_logico = bloqueSegunPuntero(puntero);
		int bloque_fisico = bloqueLogicoAFisico(fcb, bloque_logico);
		int offset = offsetSegunPuntero(puntero);
		int posicion = posicionArchivoBloques(bloque_fisico, offset);
		leerArchivoBloques(aLeer + tam_leido, posicion, tam_a_leer);

		usleep(retardo_acceso_bloque);
		log_info(fileSystem_logger, "Acceso Bloque - Archivo:%s - Bloque Archivo:%d - Bloque File System: %d", archivo, bloque_logico, bloque_fisico);

		tam_leido += tam_a_leer;
		puntero += tam_a_leer;
	}

}






void ejecutar_instruccion(char* instruccion){
	char** parametros = string_split(instruccion, " ");
	codigo_instruccion cod_instruccion = obtener_codigo_instruccion(parametros[0]);

	switch(cod_instruccion) {
		case F_OPEN:
			parametros = string_split(instruccion, " ");
			char* archivo_abrir = parametros[1];
			//printf("se solicito abrir el archivo: %s \n", archivo_abrir);
			if(existe_archivo(archivo_abrir)){
				log_info(fileSystem_logger, "abrir archivo: %s",archivo_abrir);
				enviar_mensaje("OK", socket_Kernel);
			}else{
				enviar_mensaje("NO EXISTE", socket_Kernel);
			}
			break;
		case F_CREATE:
			parametros = string_split(instruccion, " ");
			char* archivo_crear = parametros[1];
			crear_archivo(archivo_crear);
			log_info(fileSystem_logger, "crear archivo: %s", archivo_crear);
			enviar_mensaje("OK", socket_Kernel);
			break;
		case F_TRUNCATE:

			char* archivo_truncar = parametros[1];
			int tamanio = atoi(parametros[2]);
			log_info(fileSystem_logger, "Truncar archivo: %s tamanio: %d", archivo_truncar, tamanio);
			cambiar_tamanio(archivo_truncar, tamanio);
			//log_info(fileSystem_logger, "truncar archivo: %s - tamaño: %d", archivo_truncar, tamanio);
			enviar_mensaje("el filesystem trunco el archivo", socket_Kernel);
			break;
		case F_WRITE:

			char mensaje[100]="";
			char* direccion_fisica = parametros[2];
			int tamanio_write = atoi(parametros[3]);
			int puntero_write = atoi(parametros[4]);
			char* archivo_write = parametros[1];
			log_info(fileSystem_logger, "Escribir Archivo: %s - Puntero: %d- Memoria: %s - Tamaño: %d", archivo_write, puntero_write, direccion_fisica, tamanio_write);

			sprintf(mensaje, "F_WRITE %s %d", direccion_fisica,tamanio_write);
			enviar_mensaje(mensaje, socket_memoria);
			char* aEscribir = recibirMensaje(socket_memoria, fileSystem_logger);
			escribir(archivo_write, (void*)aEscribir, puntero_write, tamanio_write);
			enviar_mensaje("se escribio el archivo", socket_Kernel);
			free(aEscribir);
			//free(mensaje);
			break;
		case F_READ:

			char* archivo_read = parametros[1];
			char* direccion_fisica_read = parametros[2];
			int tamanio_read = atoi(parametros[3]);
			int puntero_read = atoi(parametros[4]);
			log_info(fileSystem_logger, "Leer Archivo: %s - Puntero: %d -Memoria: %s - Tamaño: %d", archivo_read, puntero_read, direccion_fisica_read, tamanio_read);

			char* aLeer = malloc(tamanio_read + 1);
			char mensaje_read[100]= "";
			leer(archivo_read, (void*)aLeer, puntero_read, tamanio_read);
			aLeer[tamanio_read] = '\0';
			sprintf(mensaje_read, "MOV_OUT %s %s %d", direccion_fisica_read, aLeer, tamanio_read);
			enviar_mensaje(mensaje_read, socket_memoria);
			enviar_mensaje("se leyo el archivo", socket_Kernel);
			//free(mensaje_read);
			free(aLeer);
			break;
		default:
			break;
	}
	string_iterate_lines(parametros, (void*) free);
	free(parametros);
}
