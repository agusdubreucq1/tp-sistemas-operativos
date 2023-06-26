#ifndef CICLO_INSTRUCCION_H_
#define CICLO_INSTRUCCION_H_

#include "registros_cpu.h"
#include "mmu.h"
#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <utils/instruccion.h>
#include <utils/datos.h>
#include <unistd.h>
#include <utils/cliente.h>
#include <utils/serializar.h>
#include <string.h>
#include <utils/servidor.h>

// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------

	extern t_log* cpu_logger;

// ------------------------------------------------------------------------------------------
// -- Contexto --
// ------------------------------------------------------------------------------------------

	extern t_contexto_ejecucion* contexto_de_ejecucion;
	extern u_int32_t retardo_instruccion;

// ------------------------------------------------------------------------------------------
// -- Server del proceso --
// ------------------------------------------------------------------------------------------

	extern int socket_Kernel;
	extern int socket_memoria;;


// ------------------------------------------------------------------------------------------
// -- Funciones --
// ------------------------------------------------------------------------------------------

	void comenzar_ciclo_instruccion();
	t_instruccion* fetch_instruccion();
	int ejecutar_instruccion(t_instruccion*);
	void enviarContexto(char*);
	char* concatenar_mensaje_con_3_parametros(char mensaje[30], t_instruccion* instruccion);
	char* concatenar_mensaje_con_2_parametros(char mensaje[30], t_instruccion* instruccion);
	char* concatenar_mensaje_con_1_parametro(char mensaje[30], t_instruccion* instruccion);
	void liberar_contexto(t_contexto_ejecucion* contexto);
	void liberar_elemento_list(void* elemento);

#endif /* CICLO_DE_INSTRUCCION_H_ */
