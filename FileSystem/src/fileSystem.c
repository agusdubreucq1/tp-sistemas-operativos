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

	server_fileSystem = iniciar_servidor(IP_SERVER, puerto_escucha, fileSystem_logger);
	log_info(fileSystem_logger, "Servidor listo para recibir al cliente");


	inicializar_estructuras();

	pthread_create(&conexionMemoria, NULL, conectarMemoria, NULL);
	pthread_detach(conexionMemoria);
	pthread_create(&atender_kernel, NULL, abrirSocketKernel, NULL);
	pthread_join(atender_kernel, NULL);

	//cerrar_fileSystem();
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
	return "";
}

void recibir_mensaje_kernel(){
	int cod_op;
	cod_op = recibir_operacion(socket_Kernel);
	switch (cod_op) {
		case MENSAJE:

			char* mensaje = string_new();
			string_append(&mensaje, recibir_instruccion(socket_Kernel, fileSystem_logger));
			ejecutar_instruccion(mensaje);
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
			string_append(&mensaje, recibir_instruccion(socket_memoria, fileSystem_logger));

			break;
		default: break;
	}
}

void inicializar_estructuras(){

	//sem_wait(&espera_cerrar);

	levantar_superBloque(path_superbloque, fileSystem_logger);
	inicializar_bitmap();
	inicializar_bloques();
	inicializar_FCBs();
	printf("todo creado");
	//leer_fcb(prueba);

}

void inicializar_FCBs(){
	lista_fcb = list_create();
	DIR *d;
	struct dirent *dir;
	    d = opendir(path_fcb);
	    if (d) {
	        while ((dir = readdir(d)) != NULL) {
	        	if (dir->d_type == DT_REG) {
					printf("%s\n", dir->d_name);
					char* path = string_new();
					string_append(&path, path_fcb);
					string_append(&path, "/");
					string_append(&path, dir->d_name);

					t_config* fcb_config = config_create(path);
					char* nombre = config_get_string_value(fcb_config, "NOMBRE_ARCHIVO");
					uint32_t tamanio = config_get_int_value(fcb_config, "TAMANIO_ARCHIVO");
					uint32_t puntero_directo = config_get_int_value(fcb_config, "PUNTERO_DIRECTO");
					uint32_t puntero_indirecto = config_get_int_value(fcb_config, "PUNTERO_INDIRECTO");
					printf("nombre: %s\n tamanio: %d\n pd: %d\n pi: %d\n", nombre, tamanio, puntero_directo, puntero_indirecto);

					t_fcb* fcb = leer_fcb(nombre, tamanio, puntero_directo, puntero_indirecto);
					list_add(lista_fcb, fcb);
					config_destroy(fcb_config);
					free(path);
	        	}
	        }
	        closedir(d);
	    }
}

t_fcb* leer_fcb(char* nombre, uint32_t tamanio, uint32_t puntero_directo, uint32_t puntero_indirecto){
	t_fcb* fcb;
	fcb = malloc(sizeof(t_fcb));
	fcb->nombre_archivo = string_new();
	string_append(&fcb->nombre_archivo, nombre);
	fcb->tamano_archivo = tamanio;
	fcb->puntero_directo = puntero_directo;
	fcb->puntero_indirecto = puntero_indirecto;
	return fcb;
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

void cambiar_tamanio(char* archivo, int tamanio){
	t_fcb* fcb = fcb_segun_nombre(archivo);
	if(tamanio > fcb->tamano_archivo){
		//agregar bloques
	}else{
		//liberar bloques
	}
	fcb->tamano_archivo = tamanio;
}

t_fcb* fcb_segun_nombre(char* archivo){
	for(int i=0;i<list_size(lista_fcb);i++){
		t_fcb* fcb = list_get(lista_fcb, i);
		if(string_equals_ignore_case(fcb->nombre_archivo, archivo)){
			return fcb;
		}
	}
	return NULL;
}

void ejecutar_instruccion(char* instruccion){

	char** parametros = string_split(instruccion, " ");
	codigo_instruccion cod_instruccion = obtener_codigo_instruccion(parametros[0]);

	switch(cod_instruccion) {
		case F_OPEN:
			parametros = string_split(instruccion, " ");
			char* archivo_abrir = parametros[1];
			printf("se solicito abrir el archivo: %s \n", archivo_abrir);
			if(existe_archivo(archivo_abrir)){
				printf("el archivo: %s ya existia\n", archivo_abrir);
			}else{
				crear_archivo(archivo_abrir);
				printf("el archivo: %s fue creado", archivo_abrir);
			}
			enviar_mensaje("el filesystem abrio el archivo", socket_Kernel);
			break;
		case F_TRUNCATE:
			char* archivo_truncar = parametros[1];
			int tamanio = atoi(parametros[2]);
			printf("se solicito truncar el archivo: %s", archivo_truncar);
			cambiar_tamanio(archivo_truncar, tamanio);
			enviar_mensaje("el filesystem trunco el archivo", socket_Kernel);
			break;
		case F_WRITE:
			break;
		case F_READ:
			break;
		default:
			break;
	}

}


