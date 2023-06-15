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

	sem_init(&espera_cerrar, 0, 0);
	pthread_create(&conexionMemoria, NULL, conectarMemoria, NULL);
	pthread_detach(conexionMemoria);

	pthread_create(&atender_kernel, NULL, abrirSocketKernel, NULL);
	pthread_detach(atender_kernel);

	inicializar_estructuras();
	cerrar_fileSystem();
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

	while(1){
		recibir_mensaje_memoria();
	}

	return "";
}

void recibir_mensaje_kernel(){
	int cod_op;
	cod_op = recibir_operacion(socket_Kernel);
	switch (cod_op) {
		case MENSAJE:
			char* mensaje = "";
			strcat(mensaje, recibir_instruccion(socket_Kernel, fileSystem_logger));

			break;
		default: break;
	}
}

void recibir_mensaje_memoria(){
	int cod_op;
	cod_op = recibir_operacion(socket_memoria);
	switch (cod_op) {
		case MENSAJE:
			char* mensaje = "";
			strcat(mensaje, recibir_instruccion(socket_memoria, fileSystem_logger));

			break;
		default: break;
	}
}

void inicializar_estructuras(){

	sem_wait(&espera_cerrar);

	levantar_superBloque(path_superbloque, fileSystem_logger);
	inicializar_bitmap();
	inicializar_bloques();
	//leer_fcb(prueba);

}


void ejecutar_instruccion(char* instruccion){

	char** parametros = string_split(instruccion, " ");
	codigo_instruccion cod_instruccion = obtener_codigo_instruccion(parametros[0]);

	switch(cod_instruccion) {

	case F_OPEN:
		parametros = string_split(instruccion, " ");
		break;
	default:
		break;
	}
}

void cerrar_fileSystem(){
	sem_wait(&espera_cerrar);
}
/*




void crear_bloques(char* path_bloques, t_superBloque* superbloque, int retardo, t_log* fileSystem_logger) {
    int archivo_bloques_tam = superbloque->tamanio_bloque * superbloque->cant_bloques;

    FILE* archivo = fopen(path_bloques, "r+b");
    if (archivo == NULL) {
        log_error(fileSystem_logger, "Error al abrir el archivo de bloques");
        return;
    }

    void* mmapBlocks = malloc(archivo_bloques_tam);
    if (mmapBlocks == NULL) {
        log_error(fileSystem_logger, "Error al asignar memoria para mmapBlocks");
        fclose(archivo);
        return;
    }

    fread(mmapBlocks, archivo_bloques_tam, 1, archivo);

    void* copia_bloques = malloc(archivo_bloques_tam);
    if (copia_bloques == NULL) {
        log_error(fileSystem_logger, "Error al asignar memoria para copia_bloques");
        fclose(archivo);
        free(mmapBlocks);
        return;
    }
    int bloques_mapeados = 0;
    //int retardoenSegundos = retardo_en_segundos(retardo);
    // no sé cómo hacer que pare el while si le pongo un 1 entonces pongo un criterio de paro
    while(bloques_mapeados < superbloque->cant_bloques) {
       // sleep(retardoenSegundos );
        log_info(fileSystem_logger, "Logueo por cada bloque");
        memcpy(copia_bloques, mmapBlocks, archivo_bloques_tam);

        fseek(archivo, 0, SEEK_SET);
        fwrite(copia_bloques, archivo_bloques_tam, 1, archivo);

        fflush(archivo);
        bloques_mapeados++;
    }

    free(copia_bloques);
    free(mmapBlocks);
    fclose(archivo);
}

int retardo_en_segundos(int milisegundos){
	return milisegundos/1000;
}
*/

