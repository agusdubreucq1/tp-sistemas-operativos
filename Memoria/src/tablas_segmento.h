/*
 * tablas_segmento.h
 *
 *  Created on: Jun 15, 2023
 *      Author: utnso
 */

#ifndef TABLAS_SEGMENTO_H_
#define TABLAS_SEGMENTO_H_

#include <utils/datos.h>
#include <stdint.h>
#include <commons/collections/list.h>
#include <stdlib.h>
#include <utils/serializar.h>
#include <commons/string.h>
#include "bitmap_memoria.h"
#include <commons/bitarray.h>

// ------------------------------------------------------------------------------------------
// -- Variables --
// ------------------------------------------------------------------------------------------

	extern 	t_log* memoria_logger;
	extern char* cant_segmentos;
	extern t_segmento* segmento_cero;
	extern int memoria_libre;
	extern char *algoritmo_asignacion;
	extern void* memoria_fisica;
	extern t_bitarray* bitmap;
	extern t_list* tablas_segmentos;
	extern t_list* lista_huecos;
	extern alg_asignacion algoritmo;
	extern t_list* tablas_segmentos;
	extern void* memoria_fisica;
	extern char* tam_memoria;

// ------------------------------------------------------------------------------------------
// -- Funciones --
// ------------------------------------------------------------------------------------------

	t_tabla_segmentos* crear_tabla(uint32_t pid);
	t_tabla_segmentos* buscar_tabla_proceso(uint32_t pid);
	t_segmento* crear_segmento(void* base, void* limite, uint32_t id, bool libre, uint32_t pid);
	void enviar_segmentos(t_tabla_segmentos* tabla, int socket);
	alg_asignacion obtener_algoritmo_asignacion(char*);
	char* elegir_hueco(int size);
	void borrar_segmento(t_segmento* segmento);
	void borrar_tabla(t_tabla_segmentos* tabla);
	int buscar_index_proceso(uint32_t pid);
	void insertar_ordernado(t_segmento* hueco, t_list* lista, bool unificar);
	void imprimir_huecos(t_list* lista);
	void liberar_elemento(void* elemento);
	void fin_segmento(t_segmento* segmento, int index);
	void unificar_espacios();
	void compactar_perrito();
	void compacta(t_list* lista);
	bool comparator(void *a, void *b);



#endif /* TABLAS_SEGMENTO_H_ */
