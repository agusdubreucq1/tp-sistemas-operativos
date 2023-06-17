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
	list_add_in_index(tabla->segmentos,0, segmento_cero);

	for(int i = 1; i < atoi(cant_segmentos); i++){
		//t_segmento* segmento = malloc(sizeof(t_segmento));
		t_segmento* segmento = crear_segmento(NULL, NULL);
		list_add_in_index(tabla->segmentos,i, segmento);
	}
	return tabla;
}

t_segmento* crear_segmento(void* base, void* limite){
	t_segmento* segmento = malloc(sizeof(t_segmento));
	segmento->direccion_base = base;
	segmento->limite = limite;
	memoria_libre -= (limite - base);
	//printf("\nMemoria Libre %d \n", memoria_libre);
	alg_asignacion algoritmo = obtener_algoritmo_asignacion(algoritmo_asignacion);

	switch(algoritmo) {

		case FIRST:

			break;

		case BEST:
			printf("Entre a BEST");
			break;

		case WORST:

			break;
	}

	return segmento;
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
	if(string_equals_ignore_case(algoritmo, "WORST"))	return WORST;
}
