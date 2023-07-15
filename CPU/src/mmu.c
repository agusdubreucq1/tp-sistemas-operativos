#include "mmu.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int obtener_num_segmento(int direccion_logica){

	return floor(direccion_logica/atoi(tam_max_segmento));

}

int obtener_desplazamiento_segmento(int direccion_logica){

	return direccion_logica%atoi(tam_max_segmento);

}


int desplazamiento_valido(t_registros *registros, char *registro, int direccion_logica){

	int desplazamiento_segmento = obtener_desplazamiento_segmento(direccion_logica)+registros_get_size(registros, registro);
	t_segmento* segmento = list_get(contexto_de_ejecucion->tabla_segmentos->segmentos, obtener_num_segmento(direccion_logica));
	int limite = (intptr_t)segmento->limite;
	int base = (intptr_t)segmento->direccion_base;

	/*printf("\nDesplazamiento + Tamanio_a_leer/escribir de %s: %d + %d = %d\n", registro,
																			   obtener_desplazamiento_segmento(direccion_logica),
																			   registros_get_size(registros, registro),
																			   desplazamiento_segmento);*/

	if((base+desplazamiento_segmento) > limite) {
		return 0;
	}

	return 1;
}

void* direccion_fisica(int direccion_logica){
	int num_segmento = obtener_num_segmento(direccion_logica);
	int offset = obtener_desplazamiento_segmento(direccion_logica);
	t_segmento* segmento = list_get(contexto_de_ejecucion->tabla_segmentos->segmentos, num_segmento);
	void* base = segmento->direccion_base;
	log_trace(cpu_logger, "base: %p + offset: %d = %p", base, offset, base + offset);
	return base + offset;
}
