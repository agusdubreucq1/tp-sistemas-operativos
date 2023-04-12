#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>

	extern t_log* memoria_logger;

// ------------------------------------------------------------------------------------------
// -- Configs --
// ------------------------------------------------------------------------------------------
	extern t_config* consola_config;

t_log* iniciar_logger(char* path, char* nombre_modulo);

#endif
