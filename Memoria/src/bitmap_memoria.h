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
// -- Variables --
// ------------------------------------------------------------------------------------------

	extern char* tam_memoria;
	extern t_bitarray* bitmap;

// ------------------------------------------------------------------------------------------
// -- Funciones --
// ------------------------------------------------------------------------------------------

void inicializar_bitmap();
void imprimir_bitmap(t_bitarray* bitmap);

#endif /* BITMAP_MEMORIA_H_ */
