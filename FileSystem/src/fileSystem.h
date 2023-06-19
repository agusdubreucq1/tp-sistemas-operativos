#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "config.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/log.h>
#include <commons/bitarray.h>
#include <utils/utils.h>
#include <utils/servidor.h>
#include <utils/cliente.h>
#include <utils/datos.h>
#include "superbloque.h"
#include "bitmap.h"
#include "bloques.h"
#include "fcb.h"
#include <utils/instruccion.h>
#include <semaphore.h>
#include <math.h>

#define IP_SERVER "127.0.0.1"

// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------

	t_log* fileSystem_logger;

// ------------------------------------------------------------------------------------------
// -- Config del proceso --
// ------------------------------------------------------------------------------------------

	t_config* fileSystem_config;
	char *ip_memoria, *puerto_memoria, *puerto_escucha, *path_superbloque;
	char *path_bitmap, *path_bloques, *path_fcb;
	u_int32_t retardo_acceso_bloque;
	t_list* lista_fcb;

// ------------------------------------------------------------------------------------------
// -- Socket del proceso --
// ------------------------------------------------------------------------------------------

	int server_fileSystem;
	int socket_memoria;
	int socket_Kernel;
	uint32_t respuesta;
	pthread_t atender_kernel;
	pthread_t conexionMemoria;

// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

	void* abrirSocketKernel();
	void* conectarMemoria();
	void recibir_mensaje_kernel();
	void inicializar_estructuras();

// ------------------------------------------------------------------------------------------
// -- SUPER BLOQUE--
// ------------------------------------------------------------------------------------------
		int tamanio_bloque;
		int cant_bloques;


// ------------------------------------------------------------------------------------------
// -- BITMAP de BLOQUES--
// ------------------------------------------------------------------------------------------
	t_bitarray* bitmap;


	void recibir_mensaje_memoria();
	void cerrar_fileSystem();
	void ejecutar_instruccion(char* instruccion);
	void inicializar_FCBs();
	t_fcb* leer_fcb(char* nombre, uint32_t tamanio, uint32_t puntero_directo, uint32_t puntero_indirecto);
	int existe_archivo(char* nombre);
	void crear_archivo(char* nombre);
	t_fcb* fcb_segun_nombre(char* archivo);
	int redondearArriba(double x);

#endif /* FILESYSTEM_H_ */
