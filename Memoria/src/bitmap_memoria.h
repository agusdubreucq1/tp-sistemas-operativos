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

// ------------------------------------------------------------------------------------------
// -- Funciones --
// ------------------------------------------------------------------------------------------

void inicializar_bitmap();
void imprimir_bitmap(t_bitarray* bitmap);
int validar_huecos_libres(t_bitarray*, int, int);
void ocupar_bitmap(t_bitarray*, int, int);
void liberar_bitmap(t_bitarray*, int, int);
int first_fit_bitmap(t_bitarray*, int);
int best_fit_bitmap(t_bitarray*, int);
int worst_fit_bitmap(t_bitarray*, int);

#endif /* BITMAP_MEMORIA_H_ */
