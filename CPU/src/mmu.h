#ifndef MMU_H_
#define MMU_H_
//#include "cpu.h"

// ------------------------------------------------------------------------------------------
// -- Variable del proceso --
// ------------------------------------------------------------------------------------------

	extern char *tam_max_segmento;

// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

	int obtener_num_segmento(int);
	int obtener_desplazamiento_segmento(int);
	int desplazamiento_valido(char*, int);

#endif
