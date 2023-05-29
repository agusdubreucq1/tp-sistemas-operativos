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

	void registros_put(t_registros* registros, char* registro, char* nuevo_valor);
	void imprimir_registros(t_registros* registros);
	void iniciar_registros(t_registros* registros);

#endif
