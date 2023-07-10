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
	log_info(memoria_logger, "Creación de Proceso PID: %u", pid);
	for(int i = 1; i < atoi(cant_segmentos); i++){
		t_segmento* segmento = malloc(sizeof(t_segmento));
		segmento = crear_segmento(NULL, NULL, i, 1, pid);
		list_add_in_index(tabla->segmentos,i, segmento);
	}
	return tabla;
}

void borrar_tabla(t_tabla_segmentos* tabla){
	for(int i = 1; i < atoi(cant_segmentos); i++){
		t_segmento* segmento = list_get(tabla->segmentos,i);
		borrar_segmento(segmento);
	}
}

t_tabla_segmentos* buscar_tabla_proceso(uint32_t pid){
	t_tabla_segmentos* tabla;
	//= malloc(sizeof(t_tabla_segmentos));
	for (int i=0; i< list_size(tablas_segmentos); i++){
		tabla = list_get(tablas_segmentos, i);
		if (tabla->pid == pid){
			return tabla;
		}
	}
	return NULL;
}

int buscar_index_proceso(uint32_t pid){
	t_tabla_segmentos* tabla = malloc(sizeof(t_tabla_segmentos));
	for (int i=0; i< list_size(tablas_segmentos); i++){
		tabla = list_get(tablas_segmentos, i);
		if (tabla->pid == pid){
			return i;
		}
	}
	return -1;
}

t_segmento* crear_segmento(void* base, void* limite, uint32_t id, bool libre, uint32_t pid){
	t_segmento* segmento = malloc(sizeof(t_segmento));
	segmento->direccion_base = base;
	segmento->limite = limite;
	segmento->id = id;
	segmento->libre = libre;
	segmento->pid = pid;
	if (libre == 0){
		memoria_libre -= (limite - base);
	}
	//ocupar_bitmap((base - memoria_fisica), (limite - base));
	//printf("\nMemoria Libre %d \n", memoria_libre);
	return segmento;
}


/*void borrar_segmento(void* base, void* limite){
	memoria_libre += (limite - base);
	liberar_bitmap((base - memoria_fisica), (limite - base));
	//printf("\nMemoria Libre %d \n", memoria_libre);
}*/

void borrar_segmento(t_segmento* segmento){
	memoria_libre += (segmento->limite - segmento->direccion_base);
	t_segmento* hueco_nuevo = malloc(sizeof(t_segmento));
	hueco_nuevo->direccion_base = segmento->direccion_base;
	hueco_nuevo->limite = segmento->limite;
	hueco_nuevo->id = 0;
	hueco_nuevo->pid = 0;
	hueco_nuevo->libre = 1;
	segmento->direccion_base = NULL;
	segmento->limite = NULL;
	segmento->libre = 1;
	insertar_ordernado(hueco_nuevo);
}

void insertar_ordernado(t_segmento* hueco){
	int elementos = list_size(lista_huecos);
	for (int i = 0; i < elementos; i++) {
		t_segmento* segmento = list_get(lista_huecos, i);
		if (hueco->direccion_base < segmento->direccion_base){
			if (hueco->limite == segmento->direccion_base){
				segmento->direccion_base = hueco->direccion_base;
				if(i > 0){
					t_segmento* segmento_anterior = list_get(lista_huecos, i-1);
					if (segmento->direccion_base == segmento_anterior->limite){
						segmento_anterior->limite = segmento->limite;
						list_remove(lista_huecos, i);
					}
				}
				free(hueco);
				return;
			} else {
				list_add_in_index(lista_huecos, i, hueco);
				return;
			}
		}
	}
	list_add(lista_huecos, hueco);
}


char* elegir_hueco(int size){
	if (size <= memoria_libre){
		void* retorno;

		switch(algoritmo) {
			case FIRST:
				retorno = first_fit(size);
				imprimir_huecos();
				break;
			case BEST:
				//retorno = best_fit_bitmap(size);
				break;
			case WORST:
				//retorno = worst_fit_bitmap(size);
				break;
		}

		if (retorno == NULL){
			return "COMPACT";
		} else {
			char* num = malloc(20 * sizeof(char));
			sprintf(num, "%p", retorno);
			printf("\n NUM %s \n", num);
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

void imprimir_huecos(){
	int elementos = list_size(lista_huecos);
	for (int i = 0; i < elementos; i++) {
		t_segmento* segmento = list_get(lista_huecos, i);
		printf("\n\n Hueco %i", i);
		printf("\n Base %p", segmento->direccion_base);
		printf("\n Limite %p", segmento->limite);
	}
}
