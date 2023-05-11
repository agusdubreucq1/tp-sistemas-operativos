#ifndef INSTRUCCIONES_H_
#define INSTRUCCIONES_H_
/*

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <utils/buffer.h>

//#include "pcb.h"
#include "string.h"
//#include "buffer.h"


typedef enum {
	 SET,
	 MOV_IN,
	 MOV_OUT,
	 I_O,
	 F_OPEN,
	 F_CLOSE,
	 F_SEEK,
	 F_READ,
	 F_WRITE,
	 F_TRUNCATE,
	 WAIT,
	 SIGNAL,
	 CRATE_SEGMENT,
	 DELETE_SEGMENT,
	 YIELD,
	 EXITT //TODO VER EXIT

}cod_instruccion;

typedef struct {
    u_int32_t cod_instruccion;
    char* parametro[3]; //Establecemos 3 parametros (el maximo posible), De lo contario, tendrán valores NULL.
}t_instruccion;

char* leer_archivo(char* path);
t_list* crear_lista_de_instrucciones(char* buffer);
void eliminar_instruccion(t_instruccion* instruccion);
cod_instruccion obtener_codigo_instruccion(char* co_inst);
t_instruccion* crear_instruccion_parseando_linea(char* linea);
char* obtener_string_codigo_instruccion(cod_instruccion co_inst);
void imprimir_lista_instrucciones(t_list *lista_de_instrucciones);

t_buffer* instruccion_serializar(t_instruccion* it);
t_instruccion* instruccion_deserializar(t_buffer** it);

*/
#endif /* INSTRUCCIONES_H_ */
