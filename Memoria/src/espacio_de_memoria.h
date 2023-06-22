#ifndef ESPACIO_DE_MEMORIA_H_
#define ESPACIO_DE_MEMORIA_H_

#include "bitmap_memoria.h"

// ------------------------------------------------------------------------------------------
// -- Variable del proceso --
// ------------------------------------------------------------------------------------------

	extern void* memoria_fisica;
	extern char* tam_memoria;

// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

	void* reservar_espacio_memoria();
	void imprimir_memoria_segun_base_y_tam(void *, int);
	void imprimir_memoria();
	//void escribir_mensaje_en_memoria(void *, char *, int);

#endif
