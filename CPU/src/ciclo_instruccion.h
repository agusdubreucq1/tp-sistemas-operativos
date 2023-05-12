#ifndef CICLO_INSTRUCCION_H_
#define CICLO_INSTRUCCION_H_

#include "registros_cpu.h"
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

// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------

	extern t_log* cpu_logger;

// ------------------------------------------------------------------------------------------
// -- Contexto --
// ------------------------------------------------------------------------------------------

	extern t_contexto_ejecucion* contexto_de_Ejecucion;
	extern u_int32_t retardo_instruccion;

// ------------------------------------------------------------------------------------------
// -- Server del proceso --
// ------------------------------------------------------------------------------------------

	extern int socket_Kernel;


// ------------------------------------------------------------------------------------------
// -- Funciones --
// ------------------------------------------------------------------------------------------

	void comenzar_ciclo_instruccion();
	t_instruccion* fetch_instruccion();
	void ejecutar_instruccion(t_instruccion* instruccion);
	void enviarContexto();

#endif /* CICLO_DE_INSTRUCCION_H_ */
