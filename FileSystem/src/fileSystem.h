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
// -- Estructuras FS--
// ------------------------------------------------------------------------------------------

	int tamanio_bloque;
	int cant_bloques;
	t_bitarray* bitmap;
	t_list* lista_fcb;
	sem_t espera_cerrar;


// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

	void* abrirSocketKernel();
	void recibir_mensaje_kernel();
	void inicializar_estructuras();
	void* conectarMemoria();
	void recibir_mensaje_memoria();
	void cerrar_fileSystem();
	void ejecutar_instruccion(char* instruccion);
	int existe_archivo(char* nombre);
	void crear_archivo(char* nombre);
	void leer(char* archivo, void* aLeer, int puntero, int tamanio);
	void escribir(char* archivo, void* aEscribir, int puntero, int tamanio);
	int posicionArchivoBloques(int num_bloque, int offset);
	int bloqueLogicoAFisico(t_fcb* fcb, int num_bloque);
	int offsetSegunPuntero(int puntero);
	int bloqueSegunPuntero(int puntero);
	int min(int num1, int num2);
	void achicar(t_fcb* fcb, int asignados, int necesarios);
	void liberar_bloque(t_fcb* fcb, int* asignados);
	void agrandar(t_fcb* fcb, int asignados, int necesarios);
	void agregar_bloque(t_fcb* fcb, int* asignados);



// ------------------------------------------------------------------------------------------
// -- SUPER BLOQUE--
// ------------------------------------------------------------------------------------------
		int tamanio_bloque;
		int cant_bloques;


// ------------------------------------------------------------------------------------------
// -- BITMAP de BLOQUES--
// ------------------------------------------------------------------------------------------
	t_bitarray* bitmap;


#endif /* FILESYSTEM_H_ */
