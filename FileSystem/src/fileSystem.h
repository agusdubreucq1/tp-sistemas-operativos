/*
 * filesystem.h
 *
 *  Created on: Apr 12, 2023
 *      Author: utnso
 */

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "logger.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>

// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------

	t_log* fileSystem_logger;

// ------------------------------------------------------------------------------------------
// -- Config del proceso --
// ------------------------------------------------------------------------------------------

	t_config* fileSystem_config;

	char *file_ip_memoria, *puerto_memoria, *puerto_escucha, *path_superbloque;
	char *file_path_bitmap, *path_bloques, *path_fcb, *retardo_acceso_bloque;

#endif /* FILESYSTEM_H_ */
