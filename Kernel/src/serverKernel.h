/*
 * socketKernel.h
 *
 *  Created on: Apr 10, 2023
 *      Author: utnso
 */

#ifndef SERVERKERNEL_H_
#define SERVERKERNEL_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>

#define IP "127.0.0.1"

typedef enum
{
	MENSAJE,
	PAQUETE
}op_code;

// ------------------------------------------------------------------------------------------
// -- Server del proceso --
// ------------------------------------------------------------------------------------------

	extern uint32_t handshake;

// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------

	extern t_log* kernel_logger;

// ------------------------------------------------------------------------------------------
// -- Config del proceso --
// ------------------------------------------------------------------------------------------

	extern u_int32_t grado_maximo_multiprogramacion;
	extern char *ip_memoria, *puerto_memoria, *ip_filesystem, *puerto_filesystem, *ip_cpu;
	extern char *puerto_cpu, *puerto_escucha, *estimacion_inicial, *hrrn_alfa;
	extern char *algoritmo_planificacion, **recursos, **instancias_recursos;

// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

void* recibir_buffer(int*, int);
int iniciar_servidor(void);
int esperar_cliente(int);
t_list* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);
//void verificarHandshake(int conexion, uint32_t respuestaOk, uint32_t resultOk, uint32_t resultError);

#endif /* SERVERKERNEL_H_ */
