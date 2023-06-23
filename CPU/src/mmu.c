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

	printf("\nDesplazamiento + Tamanio_a_leer/escribir de %s: %d + %d = %d\n", registro,
																			   obtener_desplazamiento_segmento(direccion_logica),
																			   registros_get_size(registros, registro),
																			   desplazamiento_segmento);

	if(desplazamiento_segmento > atoi(tam_max_segmento)) {
		return 0;
	}

	return 1;
}


