#ifndef CLIENT_H_
#define CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <commons/log.h>

#include "server.h"


typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;


// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------

	extern t_log* fileSystem_logger;


// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

	void* serializar_paquete(t_paquete* paquete, int bytes);
	int crear_conexion(char* ip, char* puerto);
	void enviar_mensaje(char* mensaje, int socket_cliente);
	uint32_t handshake(int);
	t_paquete* crear_paquete(void);
	t_paquete* crear_super_paquete(void);
	void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
	void enviar_paquete(t_paquete* paquete, int socket_cliente);
	void liberar_conexion(int socket_cliente);
	void eliminar_paquete(t_paquete* paquete);


#endif /* CLIENT_H_ */
