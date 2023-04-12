/*
 * logger.h
 *
 *  Created on: Apr 12, 2023
 *      Author: utnso
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>


t_log* iniciar_logger(char* path, char* nombre_modulo);


#endif /* LOGGER_H_ */
