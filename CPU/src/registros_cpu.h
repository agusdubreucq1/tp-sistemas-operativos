#ifndef REGISTROS_CPU_H_
#define REGISTROS_CPU_H_

#include "paquete_a_enviar.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <utils/datos.h>

typedef struct registro_unitario{
	char* registro;
	uint32_t valor;
}t_registro_unitario;

extern t_registros* registros_cpu; // Variable externa de los registros


void iniciar_registros(t_registros**);
void resetear_registros(t_registros*);
void liberar_registros(t_registros*);
void imprimir_registros_por_pantalla(t_registros*);
uint32_t registros_get(t_registros* registros, char* registro);
void registros_put(t_registros* registros, char* registro, uint32_t nuevo_valor);
void actualizar_registros_cpu(t_registros* registros_cpu, t_registros* nuevos_valores);

t_registro_unitario* crear_registro_unitario();
void destruir_registro_unitario(t_registro_unitario* registro);
t_registro_unitario* recibir_registro_unitario(int socket, t_log* logger);
void imprimir_registro_unitario_por_pantalla(t_registro_unitario* registro);
void enviar_registro_unitario(char* registro, u_int32_t valor, op_code operacion, t_log* logger, int socket);

#endif
