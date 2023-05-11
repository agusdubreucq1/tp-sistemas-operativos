#include "buffer.h"

t_buffer* buffer_create() {
    return buffer_create_for(0, NULL);
}

t_buffer* buffer_create_for(int32_t tamanio, void* elemento) {
    t_buffer *buffer = malloc(sizeof(t_buffer));
    buffer->size = tamanio;
    buffer->stream = elemento;
    return buffer;
}

void buffer_destroy(t_buffer** buffer) {
    if ((*buffer) == NULL) return;
    if ((*buffer)->stream != NULL) {
        free((*buffer)->stream);
        (*buffer)->stream = NULL;
    }
    free(*buffer);
    *buffer = NULL;
}

void buffer_reducir_en(t_buffer** buffer, uint32_t bytes_a_reducir) {
    (*buffer)->size -= bytes_a_reducir;
    if ((*buffer)->size <= 0) {
        buffer_destroy(buffer);
    } else {
        void* stream = malloc((*buffer)->size);
        memcpy(stream, (*buffer)->stream + bytes_a_reducir, (*buffer)->size);
        free((*buffer)->stream);
        (*buffer)->stream = stream;
    }
}

// -------------------------------------------------------------------
// ---- Serializador ----
// -------------------------------------------------------------------

#define __buffer_serializar(tipo)                   \
    t_buffer* buffer_serializar_##tipo(tipo* it) {  \
        uint32_t size = sizeof(tipo);               \
        void* stream = malloc(size);                \
        memcpy(stream, it, size);                   \
        return buffer_create_for(size, stream);     \
    }

__buffer_serializar(char);
__buffer_serializar(int);
__buffer_serializar(long);
__buffer_serializar(float);
__buffer_serializar(double);

__buffer_serializar(int8_t);
__buffer_serializar(int16_t);
__buffer_serializar(int32_t);
__buffer_serializar(int64_t);

__buffer_serializar(uint8_t);
__buffer_serializar(uint16_t);
__buffer_serializar(uint32_t);
__buffer_serializar(uint64_t);

t_buffer* buffer_serializar_string(char* it) {
    uint32_t size = strlen(it) + 1;
    char* string = calloc(size, sizeof(char));
    strcpy(string, it);
    return buffer_create_for(size, string);
}

t_buffer* buffer_serializar_stream(t_buffer* it) {
    uint32_t size = strlen((char*) it->stream) + 1;
    void* stream = malloc(size);
    memcpy(stream, it->stream, size);
    return buffer_create_for(size, stream);
}

t_buffer* buffer_serializar_list(t_list* list, t_buffer*(*serializador_elemento)(void*)) {
    t_buffer* buffer = buffer_create();
    uint32_t cantidad_elementos = list_size(list);
    buffer_agregar_serializacion_uint32_t(buffer, &cantidad_elementos);

    void _serializar(void* elemento) {
        buffer_agregar_serializacion_struct(buffer, elemento, serializador_elemento);
    }
    list_iterate(list, _serializar);

    return buffer;
}

t_buffer* buffer_serializar_array(void** it, t_buffer*(*serializador_elemento)(void*)) {
    t_buffer* buffer = buffer_create();

    uint32_t size;
    for (size = 0; it[size] != NULL; size++);
    buffer_agregar_serializacion_uint32_t(buffer, &size);

    for (uint32_t i = 0; i < size; i++) {
        buffer_agregar_serializacion_struct(buffer, it[i], serializador_elemento);
    }

    return buffer;
}

t_buffer* buffer_serializar_string_array(char** it) {
    t_buffer* buffer = buffer_create();
    uint32_t size = string_array_size(it);

    buffer_agregar_serializacion_uint32_t(buffer, &size);
    void _serializar_string(char* string) {
        buffer_agregar_serializacion_string(buffer, string);
    }
    string_iterate_lines(it, (void*) _serializar_string);
    return buffer;
}

// -------------------------------------------------------------------
// ---- Deserializador ----
// -------------------------------------------------------------------

#define __buffer_deserializar(tipo)                         \
    tipo buffer_deserializar_##tipo(t_buffer** it) {        \
        uint32_t size = sizeof(tipo);                       \
        tipo algo;                                          \
        memcpy(&algo, (*it)->stream, size);                 \
        buffer_reducir_en(it, size);                        \
        return algo;                                        \
    }

__buffer_deserializar(char);
__buffer_deserializar(int);
__buffer_deserializar(long);
__buffer_deserializar(float);
__buffer_deserializar(double);

__buffer_deserializar(int8_t);
__buffer_deserializar(int16_t);
__buffer_deserializar(int32_t);
__buffer_deserializar(int64_t);

__buffer_deserializar(uint8_t);
__buffer_deserializar(uint16_t);
__buffer_deserializar(uint32_t);
__buffer_deserializar(uint64_t);

char* buffer_deserializar_string(t_buffer** it) {
    uint32_t size = strlen((*it)->stream) + 1;
    char* algo = malloc(size);
    memcpy(algo, (*it)->stream, size);
    buffer_reducir_en(it, size);
    return algo;
}

t_buffer* buffer_deserializar_stream(t_buffer** buffer) {
    return *buffer;
}

t_list* buffer_deserializar_list(t_buffer** buffer, void*(*deserializador_elemento)(t_buffer**)) {
    t_list* list = list_create();
    uint32_t cantidad_elementos = buffer_deserializar_uint32_t(buffer);

    for (int i = 0; i < cantidad_elementos; i++) {
        list_add(list, deserializador_elemento(buffer));
    }
    return list;
}

char** buffer_deserializar_string_array(t_buffer** buffer) {
    char** string_array = string_array_new();
    uint32_t cantidad_elementos = buffer_deserializar_uint32_t(buffer);
    for (int i = 0; i < cantidad_elementos; i++) {
        string_array_push(&string_array, buffer_deserializar_string(buffer));
    }
    return string_array;
}

void** buffer_deserializar_array(t_buffer** buffer, void*(*deserializador_elemento)(t_buffer**)) {
    int32_t cantidad_elementos = buffer_deserializar_int32_t(buffer);
    void* *array = calloc(cantidad_elementos + 1, sizeof(void*));
    for (int i = 0; i < cantidad_elementos; i++) {
        array[i] = deserializador_elemento(buffer) ;
    }
    array[cantidad_elementos] = NULL;
    return array;
}

// -------------------------------------------------------------------
// ---- Serializador encadenado ----
// -------------------------------------------------------------------

#define __buffer_agregar_serializacion(tipo)                                \
    void buffer_agregar_serializacion_##tipo(t_buffer* buffer, tipo* it) {  \
        t_buffer* buf = buffer_serializar_##tipo(it);                       \
        buffer->stream = realloc(buffer->stream, buffer->size + buf->size); \
        memcpy(buffer->stream + buffer->size, buf->stream, buf->size);      \
        buffer->size += buf->size;                                          \
        buffer_destroy(&buf);                                               \
    }

__buffer_agregar_serializacion(char);
__buffer_agregar_serializacion(int);
__buffer_agregar_serializacion(long);
__buffer_agregar_serializacion(float);
__buffer_agregar_serializacion(double);

__buffer_agregar_serializacion(int8_t);
__buffer_agregar_serializacion(int16_t);
__buffer_agregar_serializacion(int32_t);
__buffer_agregar_serializacion(int64_t);

__buffer_agregar_serializacion(uint8_t);
__buffer_agregar_serializacion(uint16_t);
__buffer_agregar_serializacion(uint32_t);
__buffer_agregar_serializacion(uint64_t);

void buffer_agregar_serializacion_string(t_buffer* buffer, char* it) {
    t_buffer* buf = buffer_serializar_string(it);
    buffer->stream = realloc(buffer->stream, buffer->size + buf->size);
    memcpy(buffer->stream + buffer->size, buf->stream, buf->size);
    buffer->size += buf->size;
    buffer_destroy(&buf);
}

void buffer_agregar_serializacion_struct(t_buffer* buffer, void* it, t_buffer*(*serializador)(void*)) {
    t_buffer* buf = serializador(it);
    buffer->stream = realloc(buffer->stream, buffer->size + buf->size);
    memcpy(buffer->stream + buffer->size, buf->stream, buf->size);
    buffer->size += buf->size;
    buffer_destroy(&buf);
}

void buffer_agregar_serializacion_list(t_buffer* buffer, t_list* list, t_buffer*(*serializador_elemento)(void*)) {
    t_buffer* buf = buffer_serializar_list(list, serializador_elemento);
    buffer->stream = realloc(buffer->stream, buffer->size + buf->size);
    memcpy(buffer->stream + buffer->size, buf->stream, buf->size);
    buffer->size += buf->size;
    buffer_destroy(&buf);
}

// -------------------------------------------------------------------
// ---- Alias ----
// -------------------------------------------------------------------

void eliminar_buffer(t_buffer* buffer) {
    buffer_destroy(&buffer);
}
