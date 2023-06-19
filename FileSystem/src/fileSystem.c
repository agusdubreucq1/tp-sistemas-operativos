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
					printf("el puntero directo es: %d\n es igual a NULL:%d\n 0==NULL: %d\n", puntero_directo, puntero_directo==NULL, 0==NULL);
					uint32_t puntero_indirecto = config_get_int_value(fcb_config, "PUNTERO_INDIRECTO");
					printf("el puntero indirecto es: %d\n", puntero_indirecto);
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
	/*si cambio a tamaño 0, me los punteros me quedarian en 0? o tendrian un numero pero se tomaria como basura*/
	t_fcb* fcb = fcb_segun_nombre(archivo);
	int bloques_asignados = redondearArriba(fcb->tamano_archivo /(float)tamanio_bloque);
	if(tamanio > fcb->tamano_archivo){
		int bloques_a_agregar = redondearArriba((tamanio - fcb->tamano_archivo)/(float)tamanio_bloque);
		for(int i=1; i<=bloques_a_agregar; i++){
			if(bloques_asignados == 0){//si no tiene asignado un puntero directo
				fcb->puntero_directo = asignar_bloque();
			}else{
				if(bloques_asignados<2){//si no tiene asignado un puntero indirecto
					fcb->puntero_indirecto = asignar_bloque();
				}
				FILE* archivo_bloques = fopen(path_bloques, "wr");
				uint32_t bloque_asignado = asignar_bloque();
				int posicion = fcb->puntero_indirecto* tamanio_bloque + (bloques_asignados - 1)*4;//en el bloque de punteros, posicion segun los bloques ya asignados
				fseek(archivo_bloques, posicion, SEEK_SET);
				fwrite(&bloque_asignado,sizeof(uint32_t),1,archivo_bloques);
				fclose(archivo_bloques);
			}
			bloques_asignados+=1;
		}

	}else{

		//liberar bloques
	}
	fcb->tamano_archivo = tamanio;
	grabar_fcb(fcb);
}

int redondearArriba(double x){
	int i = (int)x;
	return i + (x > i);
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


