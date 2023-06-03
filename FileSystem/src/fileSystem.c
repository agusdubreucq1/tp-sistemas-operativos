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

    printf("El path de superbloque es: %s\n",path_superbloque);
	t_superBloque* superbloque = levantar_superBloque(path_superbloque); //revisar si le esta llegando bien el path...
    printf("Tamaño del bloque: %d\n", superbloque->tamanio_bloque); //En el enunciado tiene que tener valor de 64
    printf("Cantidad de bloques: %d\n", superbloque->cant_bloques); //En el enunciado tiene que tener valor de 65536

    iniciar_bitmap(path_bitmap, superbloque);

    crear_bloques(path_bloques, superbloque, retardo_acceso_bloque, fileSystem_logger);

	pthread_create(&conexionMemoria, NULL, conectarMemoria, NULL);
	pthread_detach(conexionMemoria);
	pthread_create(&atender_kernel, NULL, abrirSocketKernel, NULL);
	pthread_join(atender_kernel, NULL);
	//abrirSocketKernel();

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
			recibir_instruccion(socket_Kernel, fileSystem_logger);
			break;
		default: break;
	}
}

t_superBloque* levantar_superBloque(char* path){
	t_config* configSuperBloque = config_create(path);
	t_superBloque* superbloque = malloc(sizeof(t_superBloque));
	superbloque->tamanio_bloque = config_get_int_value(configSuperBloque, "BLOCK_SIZE");
	superbloque->cant_bloques = config_get_int_value(configSuperBloque, "BLOCK_COUNT");
	config_destroy(configSuperBloque);
	return superbloque;
}


void iniciar_bitmap(char* path, t_superBloque* superbloque) {
    FILE* bitmap_archivo = fopen(path, "rb");

    if (bitmap_archivo == NULL) {
        // El archivo no existe, lo creamos
        bitmap_archivo = fopen(path, "wb");
        if (bitmap_archivo == NULL) {
            log_error(fileSystem_logger, "Error al crear el archivo");
            return;
        }

        // Rellenamos el archivo con ceros
        char zero = 0;
        for (int i = 0; i < superbloque->cant_bloques / 8; i++) {
            fwrite(&zero, sizeof(char), 1, bitmap_archivo);
        }

        // Reiniciamos el puntero de archivo al inicio
        fseek(bitmap_archivo, 0L, SEEK_SET);
    }

    int size = 0;
    char* buffer;
    fseek(bitmap_archivo, 0L, SEEK_END);
    size = ftell(bitmap_archivo);
    fseek(bitmap_archivo, 0L, SEEK_SET);

    buffer = malloc(size);
    fread(buffer, size, 1, bitmap_archivo);
    buffer = string_substring_until(buffer, size);

    bitmap = bitarray_create(buffer, superbloque->cant_bloques / 8); // creamos el array de bits, con un tamaño de cant_bloques/8 porque es el espacio en bytes que necesita el bitmap


    fclose(bitmap_archivo);

    log_info(fileSystem_logger , "Bitmap iniciado");
}

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
    int retardoenSegundos = retardo_en_segundos(retardo);
    // no sé cómo hacer que pare el while si le pongo un 1 entonces pongo un criterio de paro
    while(bloques_mapeados < superbloque->cant_bloques) {
        sleep(retardoenSegundos );
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

