#include "mmu.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int obtener_num_segmento(int direccion_logica){

	return floor(direccion_logica/atoi(tam_max_segmento));

}

int obtener_desplazamiento_segmento(int direccion_logica){

	return direccion_logica%atoi(tam_max_segmento);

}

int desplazamiento_valido(char *registro, int direccion_logica){

	int desplazamiento_segmento = obtener_desplazamiento_segmento(direccion_logica);

	if(desplazamiento_segmento >= atoi(tam_max_segmento)) { //+tamanio_a_leer -> a buscarlo a memoria
		return 0;
	}

	return 1;
}
