#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include <commons/collections/list.h>

typedef struct {
    int32_t size;
    void* stream;
} t_buffer;

t_buffer* buffer_create();
t_buffer* buffer_create_for(int32_t tamanio, void* elemento);

t_buffer* buffer_serializar_char(char* it);
t_buffer* buffer_serializar_int(int* it);
t_buffer* buffer_serializar_long(long* it);
t_buffer* buffer_serializar_float(float* it);
t_buffer* buffer_serializar_double(double* it);
t_buffer* buffer_serializar_int8_t(int8_t* it);
t_buffer* buffer_serializar_int16_t(int16_t* it);
t_buffer* buffer_serializar_int32_t(int32_t* it);
t_buffer* buffer_serializar_int64_t(int64_t* it);
t_buffer* buffer_serializar_uint8_t(uint8_t* it);
t_buffer* buffer_serializar_uint16_t(uint16_t* it);
t_buffer* buffer_serializar_uint32_t(uint32_t* it);
t_buffer* buffer_serializar_uint64_t(uint64_t* it);
t_buffer* buffer_serializar_string(char* it);
t_buffer* buffer_serializar_stream(t_buffer* it);
t_buffer* buffer_serializar_string_array(char** it);
t_buffer* buffer_serializar_list(t_list* it, t_buffer*(*serializador_elemento)(void*));
t_buffer* buffer_serializar_array(void** it, t_buffer*(*serializador_elemento)(void*));

char buffer_deserializar_char(t_buffer** buffer);
int buffer_deserializar_int(t_buffer** buffer);
long buffer_deserializar_long(t_buffer** buffer);
float buffer_deserializar_float(t_buffer** buffer);
double buffer_deserializar_double(t_buffer** buffer);
int8_t buffer_deserializar_int8_t(t_buffer** buffer);
int16_t buffer_deserializar_int16_t(t_buffer** buffer);
int32_t buffer_deserializar_int32_t(t_buffer** buffer);
int64_t buffer_deserializar_int64_t(t_buffer** buffer);
uint8_t buffer_deserializar_uint8_t(t_buffer** buffer);
uint16_t buffer_deserializar_uint16_t(t_buffer** buffer);
uint32_t buffer_deserializar_uint32_t(t_buffer** buffer);
uint64_t buffer_deserializar_uint64_t(t_buffer** buffer);
char* buffer_deserializar_string(t_buffer** it);
t_buffer* buffer_deserializar_stream(t_buffer** buffer);
char** buffer_deserializar_string_array(t_buffer** buffer);
t_list* buffer_deserializar_list(t_buffer** buffer, void*(*deserializador_elemento)(t_buffer**));
void** buffer_deserializar_array(t_buffer** buffer, void*(*deserializador_elemento)(t_buffer**));

void buffer_agregar_serializacion_char(t_buffer* buffer, char* it);
void buffer_agregar_serializacion_int(t_buffer* buffer, int* it);
void buffer_agregar_serializacion_long(t_buffer* buffer, long* it);
void buffer_agregar_serializacion_float(t_buffer* buffer, float* it);
void buffer_agregar_serializacion_double(t_buffer* buffer, double* it);
void buffer_agregar_serializacion_int8_t(t_buffer* buffer, int8_t* it);
void buffer_agregar_serializacion_int16_t(t_buffer* buffer, int16_t* it);
void buffer_agregar_serializacion_int32_t(t_buffer* buffer, int32_t* it);
void buffer_agregar_serializacion_int64_t(t_buffer* buffer, int64_t* it);
void buffer_agregar_serializacion_uint8_t(t_buffer* buffer, uint8_t* it);
void buffer_agregar_serializacion_uint16_t(t_buffer* buffer, uint16_t* it);
void buffer_agregar_serializacion_uint32_t(t_buffer* buffer, uint32_t* it);
void buffer_agregar_serializacion_uint64_t(t_buffer* buffer, uint64_t* it);
void buffer_agregar_serializacion_string(t_buffer* buffer, char* it);
void buffer_agregar_serializacion_struct(t_buffer* buffer, void* it, t_buffer*(*serializador)(void*));
void buffer_agregar_serializacion_list(t_buffer* buffer, t_list* list, t_buffer*(*serializador_elemento)(void*));

void buffer_destroy(t_buffer** buffer);
void eliminar_buffer(t_buffer* buffer);

#endif /* BUFFER_H_ */
