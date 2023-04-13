#ifndef LOGGER
#define LOGGER

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>


t_log* iniciar_logger(char* path, char* nombre_modulo);


#endif /* LOGGER_H_ */
