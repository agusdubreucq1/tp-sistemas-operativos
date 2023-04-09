#include "config.h"
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

	void terminar_programa(t_log *logger, t_config* config);//, int conexion_kernel);

