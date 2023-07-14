/*
 * bitmap.h
 *
 *  Created on: Jun 16, 2023
 *      Author: utnso
 */

#ifndef BITMAP_MEMORIA_H_
#define BITMAP_MEMORIA_H_

#include <commons/bitarray.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <utils/datos.h>

// ------------------------------------------------------------------------------------------
// -- Estructuras --
// ------------------------------------------------------------------------------------------

	typedef struct bitmap_aux {
	int posicion;
	int tamanio;
	} bitmap_aux;

// ------------------------------------------------------------------------------------------
// -- Variables --
// ------------------------------------------------------------------------------------------

	extern char* tam_memoria;
	extern t_bitarray* bitmap;
	extern void *memoria_fisica;
	extern t_list* lista_huecos;

// ------------------------------------------------------------------------------------------
// -- Funciones --
// ------------------------------------------------------------------------------------------

void inicializar_bitmap();
void imprimir_bitmap();
int validar_huecos_libres(int, int);
void ocupar_bitmap(int, int);
void liberar_bitmap(int, int);
int first_fit_bitmap(int);
int best_fit_bitmap(int);
int worst_fit_bitmap(int);
int devolver_posicion_bitmap_segun_direccion(void *);
int tamanio_segmento(t_segmento* segmento);
void* first_fit(int);
void* best_fit(int);
void* worst_fit(int);

#endif /* BITMAP_MEMORIA_H_ */
