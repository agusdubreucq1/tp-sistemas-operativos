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
		fin_segmento(segmento, i);
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
	insertar_ordernado(hueco_nuevo, lista_huecos, 1);
}

void fin_segmento(t_segmento* segmento, int index){
	if(segmento->libre == 0){
		memoria_libre += (segmento->limite - segmento->direccion_base);
		t_segmento* hueco_nuevo = malloc(sizeof(t_segmento));
		hueco_nuevo->direccion_base = segmento->direccion_base;
		hueco_nuevo->limite = segmento->limite;
		hueco_nuevo->id = 0;
		hueco_nuevo->pid = 0;
		hueco_nuevo->libre = 1;
		insertar_ordernado(hueco_nuevo, lista_huecos, 1);
	}
	list_remove_element(lista_huecos, segmento);
	//list_remove_and_destroy_element(lista_huecos, index, liberar_elemento);
}

void compactar_perrito(){
	t_list* lista_compactacion;
	lista_compactacion = list_create();

	int tablas = list_size(tablas_segmentos);
	list_add(lista_compactacion, segmento_cero);
	for (int i = 0; i < tablas; i++) {
		t_tabla_segmentos* tabla = list_get(tablas_segmentos, i);
		//imprimir_segmentos(tabla);
		int segmentos = list_size(tabla->segmentos);
		for (int j = 1; j < segmentos; j++) {
			t_segmento* segmento = list_get(tabla->segmentos, j);
			if (segmento->libre == 0){
				list_add(lista_compactacion, segmento);
			}
		}
	}

	list_sort(lista_compactacion, comparator);
	//imprimir_huecos(lista_compactacion);
	compacta(lista_compactacion);
}



void compacta(t_list* lista){
	int elementos = list_size(lista);
	void* ultimo_limite;
	for (int i = 1; i < elementos; i++) {
		t_segmento* segmento_anterior = list_get(lista, i-1);
		t_segmento* segmento = list_get(lista, i);

		if(segmento->direccion_base != segmento_anterior->limite){
			int tamanio = tamanio_segmento(segmento);
			char *valor_registro = malloc(tamanio + 1);
			memcpy(valor_registro, segmento->direccion_base, tamanio);

			segmento->direccion_base = segmento_anterior->limite;
			segmento->limite = (void*) (segmento_anterior->limite + tamanio);
			ultimo_limite = segmento->limite;

			memcpy(segmento->direccion_base, valor_registro, tamanio);
		}
	}
	t_segmento* hueco_compacto = malloc(sizeof(t_segmento));
	hueco_compacto->direccion_base =  ultimo_limite;
	hueco_compacto->limite =  (void*) (memoria_fisica + atoi(tam_memoria));
	hueco_compacto->libre =  1;
	hueco_compacto->pid =  0;
	hueco_compacto->id =  0;

	list_destroy_and_destroy_elements(lista_huecos, liberar_elemento);
	lista_huecos = list_create();
	list_add(lista_huecos, hueco_compacto);
}




void insertar_ordernado(t_segmento* hueco, t_list* lista, bool unificar){
	int elementos = list_size(lista);
	for (int i = 0; i < elementos; i++) {
		t_segmento* segmento = list_get(lista, i);
		if (hueco->direccion_base < segmento->direccion_base){
			list_add_in_index(lista, i, hueco);
		}
	}
	if (unificar == 1){
		unificar_espacios();
	}
}



void unificar_espacios(){
	int elementos = list_size(lista_huecos)-1;
	for (int i = 0; i < elementos; i++) {
		t_segmento* segmento = list_get(lista_huecos, i);
		t_segmento* segmento_sig = list_get(lista_huecos, i+1);
		if (segmento->limite == segmento_sig->direccion_base){
			segmento->limite = segmento_sig->limite;
			list_remove_element(lista_huecos, segmento_sig);
			i--;
			elementos--;
		}
	}
}


char* elegir_hueco(int size){
	if (size <= memoria_libre){
		void* retorno;

		switch(algoritmo) {
			case FIRST:
				retorno = first_fit(size);
				//imprimir_huecos();
				break;
			case BEST:
				retorno = best_fit(size);
				break;
			case WORST:
				retorno = worst_fit(size);
				break;
		}

		if (retorno == NULL){
			return "COMPACT";
		} else {
			char* num = malloc(20 * sizeof(char));
			sprintf(num, "%p", retorno);
			//printf("\n NUM %s \n", num);
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

void imprimir_huecos(t_list* lista){
	int elementos = list_size(lista);
	for (int i = 0; i < elementos; i++) {
		t_segmento* segmento = list_get(lista, i);
		/*printf("\n\n Segmento %i", i);
		printf("\n Pid %u", segmento->pid);
		printf("\n Libre %u", segmento->libre);
		printf("\n Base %p", segmento->direccion_base);
		printf("\n Limite %p", segmento->limite);
		printf("\n Tamanio hueco: %ld", segmento->limite - segmento->direccion_base);
		printf("\n");*/
	}
}

void liberar_elemento(void* elemento){
	free(elemento);
}

bool comparator(void *a, void *b) {
    t_segmento* seg_a = (t_segmento *)a;
    t_segmento* seg_b = (t_segmento *)b;
    return seg_a->direccion_base < seg_b->direccion_base;
}
