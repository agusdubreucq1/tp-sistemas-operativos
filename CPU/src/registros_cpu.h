#ifndef REGISTROS_CPU_H_
#define REGISTROS_CPU_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <utils/datos.h>


// ------------------------------------------------------------------------------------------
// -- Funciones --
// ------------------------------------------------------------------------------------------

	void registros_put(t_registros*, char*, char*);
	char* registros_get_value(t_registros*, char*);
	int registros_get_size(t_registros*, char*);
	void imprimir_registros(t_registros*);
	void iniciar_registros(t_registros*);

#endif
