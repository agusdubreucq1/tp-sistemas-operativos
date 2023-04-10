#include "config.h"
#include "parser.h"
#include "socketConsola.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <readline/readline.h>

// ------------------------------------------------------------------------------------------
// -- Logger --
// ------------------------------------------------------------------------------------------

	t_log* consola_logger;

// ------------------------------------------------------------------------------------------
// -- Configs --
// ------------------------------------------------------------------------------------------

	t_config* consola_config;
	char *ip_kernel, *puerto_kernel;

	void terminar_programa(int conexion, t_log *logger, t_config* config);

// ------------------------------------------------------------------------------------------
// -- Parser --
// ------------------------------------------------------------------------------------------

	char* buffer;

// ------------------------------------------------------------------------------------------
// -- Parser --
// ------------------------------------------------------------------------------------------

	int conexion_kernel;
	t_paquete* paquete;
