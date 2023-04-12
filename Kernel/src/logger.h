#ifndef LOGGER_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>

// ------------------------------------------------------------------------------------------
// -- Configs --
// ------------------------------------------------------------------------------------------

	extern char *ip_kernel, *puerto_kernel;

// ------------------------------------------------------------------------------------------
// -- Funciones del proceso --
// ------------------------------------------------------------------------------------------

	t_log* iniciar_logger(char* path, char* nombre_modulo);

#endif
