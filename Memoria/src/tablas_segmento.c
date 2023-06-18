/*
 * tablas_segmento.c
 *
 *  Created on: Jun 15, 2023
 *      Author: utnso
 */

#include "tablas_segmento.h"

t_tabla_segmentos* crear_tabla(uint32_t pid){
	t_tabla_segmentos* tabla = malloc(sizeof(t_tabla_segmentos));
	tabla->pid = pid;
	tabla->segmentos = list_create();
	list_add(tablas_segmentos, tabla);
	list_add_in_index(tabla->segmentos,0, segmento_cero);
	/*for(int i = 1; i < atoi(cant_segmentos); i++){
		//t_segmento* segmento = malloc(sizeof(t_segmento));
		t_segmento* segmento = crear_segmento(NULL, NULL);
		list_add_in_index(tabla->segmentos,i, segmento);
	}*/
	return tabla;
}

t_tabla_segmentos* buscar_tabla_proceso(uint32_t pid){
	t_tabla_segmentos* tabla = malloc(sizeof(t_tabla_segmentos));
	for (int i=0; i< list_size(tablas_segmentos); i++){
		tabla = list_get(tablas_segmentos, i);
		if (tabla->pid == pid){
			return tabla;
		}
	}
	return NULL;
}

t_segmento* crear_segmento(void* base, void* limite){
	t_segmento* segmento = malloc(sizeof(t_segmento));
	segmento->direccion_base = base;
	segmento->limite = limite;
	memoria_libre -= (limite - base);
	ocupar_bitmap(bitmap, (base - memoria_fisica), (limite - base));
	//printf("\nMemoria Libre %d \n", memoria_libre);
	return segmento;
}

char* elegir_hueco(int size){
	if (size <= memoria_libre){
		alg_asignacion algoritmo = obtener_algoritmo_asignacion(algoritmo_asignacion);
		int retorno;

		switch(algoritmo) {
			case FIRST:
				retorno = first_fit_bitmap(bitmap, size);
				break;
			case BEST:
				retorno = best_fit_bitmap(bitmap, size);
				break;
			case WORST:
				retorno = worst_fit_bitmap(bitmap, size);
				break;
		}

		if (retorno == -1){
			return "COMPACT";
		} else {
			char* num = malloc(20 * sizeof(char));
			sprintf(num, "%d", retorno);
			return num;
		}
	} else {
		return "OUT";
	}

}


void enviar_segmentos(t_tabla_segmentos* tabla, int socket){
	t_paquete* paquete;
	paquete = serializar_segmentos(tabla);
	int tamanio_tabla;
	memcpy(&tamanio_tabla, paquete->buffer->stream, sizeof(int));
	enviar_paquete(paquete, socket, memoria_logger, "kernel");
}

alg_asignacion obtener_algoritmo_asignacion(char* algoritmo){
	if(string_equals_ignore_case(algoritmo, "FIRST")) 	return FIRST;
	if(string_equals_ignore_case(algoritmo, "BEST"))	return BEST;
	else	return WORST;
}
