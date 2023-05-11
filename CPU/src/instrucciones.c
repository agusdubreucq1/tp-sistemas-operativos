#include "instrucciones.h"

char* obtener_string_codigo_instruccion(cod_instruccion co_inst){
	if(co_inst == SET) 			 return "SET";
	if(co_inst == MOV_IN) 		 return "MOV_IN";
	if(co_inst == MOV_OUT) 		 return "MOV_OUT";
	if(co_inst == I_O) 			 return "I/O";
	if(co_inst == F_OPEN) 		 return "F_OPEN";
	if(co_inst == F_CLOSE) 		 return "F_CLOSE";
	if(co_inst == F_CLOSE) 		 return "F_CLOSE";
	if(co_inst == F_SEEK) 		 return "F_SEEK";
	if(co_inst == F_READ) 		 return "F_READ";
	if(co_inst == F_WRITE) 		 return "F_WRITE";
	if(co_inst == F_TRUNCATE) 	 return "F_TRUNCATE";
	if(co_inst == WAIT) 		 return "WAIT";
	if(co_inst == SIGNAL) 		 return "SIGNAL";
	if(co_inst == CRATE_SEGMENT) return "CRATE_SEGMENT";
	if(co_inst == DELETE_SEGMENT)return "DELETE_SEGMENT";
	if(co_inst == YIELD) 		 return "YIELD";
	else						 return "EXITT"; //TODO VER EXIT
}



cod_instruccion obtener_codigo_instruccion(char* co_inst){
	if(string_equals_ignore_case(co_inst, "SET")) 		  return SET;
	if(string_equals_ignore_case(co_inst, "MOV_IN")) 	   return MOV_IN;
	if(string_equals_ignore_case(co_inst, "MOV_OUT")) 	   return MOV_OUT;
	if(string_equals_ignore_case(co_inst, "I/O")) 		   return I_O;
	if(string_equals_ignore_case(co_inst, "F_OPEN")) 	   return F_OPEN;
	if(string_equals_ignore_case(co_inst, "F_CLOSE")) 	   return F_CLOSE;
	if(string_equals_ignore_case(co_inst, "F_SEEK")) 	   return F_SEEK;
	if(string_equals_ignore_case(co_inst, "F_READ")) 	   return F_READ;
	if(string_equals_ignore_case(co_inst, "F_WRITE")) 	   return F_WRITE;
	if(string_equals_ignore_case(co_inst, "F_TRUNCATE"))   return F_TRUNCATE;
	if(string_equals_ignore_case(co_inst, "WAIT"))		   return WAIT;
	if(string_equals_ignore_case(co_inst, "SIGNAL"))	   return SIGNAL;
	if(string_equals_ignore_case(co_inst, "CRATE_SEGMENT"))return CRATE_SEGMENT;
	if(string_equals_ignore_case(co_inst, "DELETE_SEGMENT"))return DELETE_SEGMENT;
	if(string_equals_ignore_case(co_inst, "YIELD"))			return YIELD;
	else										 			return EXITT;//TODO VER EXIT
}


void imprimir_lista_instrucciones(t_list *lista_de_instrucciones) {

	void print_instruccion(t_instruccion *inst) {
		printf("%d - %s - %s\n", inst->cod_instruccion, inst->parametro[0],
				inst->parametro[1]);
	}
	list_iterate(lista_de_instrucciones, (void*) print_instruccion);
}

t_list* crear_lista_de_instrucciones(char* buffer){

	t_list* lista_de_instrucciones = list_create();

	//Ejemplo de Buffer: "SET AX BX\nADD AX 3\n ... EXIT\0"
	char** lines = string_split(buffer, "\n");

	//["SET AX BX", "ADD AX 3",..., "EXIT", NULL]

	void agregar_instruccion(char *linea) {
		if (!string_is_empty(linea) && !string_starts_with(linea, "#")) {
			t_instruccion* instruccion = crear_instruccion_parseando_linea(linea);
			list_add(lista_de_instrucciones, instruccion);
		}
	}
	string_iterate_lines(lines, agregar_instruccion);

//	imprimir_lista_instrucciones(lista_de_instrucciones);
	string_array_destroy(lines);
	free(buffer);

	return lista_de_instrucciones;
}



t_instruccion* crear_instruccion_parseando_linea(char* linea){
	//"SET AX BX"
	char** parametros = string_split(linea, " ");
	//["SET", "AX", "BX", NULL]
	t_instruccion* nueva_instruccion = malloc(sizeof(t_instruccion));
	nueva_instruccion->cod_instruccion = obtener_codigo_instruccion(parametros[0]);
	nueva_instruccion->parametro[0] = string_duplicate(nueva_instruccion->cod_instruccion == EXIT ? "" : parametros[1]);
	nueva_instruccion->parametro[1] = string_duplicate(nueva_instruccion->cod_instruccion == EXIT ? "" : parametros[2]);
	string_array_destroy(parametros);
	return nueva_instruccion;
}


void eliminar_instruccion(t_instruccion* instruccion){
	free(instruccion->parametro[0]);
	free(instruccion->parametro[1]);
	free(instruccion);
}

t_buffer* instruccion_serializar(t_instruccion* it) {
    t_buffer* buffer = buffer_create();
    buffer_agregar_serializacion_uint32_t(buffer, &it->cod_instruccion);
    buffer_agregar_serializacion_string(buffer, it->parametro[0]);
    buffer_agregar_serializacion_string(buffer, it->parametro[1]);
    return buffer;
}

t_instruccion* instruccion_deserializar(t_buffer** buffer) {
    t_instruccion* instruccion = malloc(sizeof(t_instruccion));
    instruccion->cod_instruccion = buffer_deserializar_uint32_t(buffer);
    instruccion->parametro[0] = buffer_deserializar_string(buffer);
    instruccion->parametro[1] = buffer_deserializar_string(buffer);
    return instruccion;
}
