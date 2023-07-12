/*
 * bitmap.h
 *
 *  Created on: Jun 13, 2023
 *      Author: utnso
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <commons/bitarray.h>
#include <commons/log.h>

//#define BITMAP_SIZE 1000
//#define FILE_NAME "bitmap.dat"

extern t_log* fileSystem_logger;
extern int cant_bloques;
extern char *path_bitmap;
extern t_bitarray* bitmap;

int inicializar_bitmap();
void imprimir_bitmap(t_bitarray* bitmap);
uint32_t asignar_bloque();
int bloque_libre();
void grabar_bitmap(t_bitarray* bitmap);
void imprimir_bitmap_20(t_bitarray* bitmap);
void desasignar_bloque(uint32_t bloque);

#endif /* BITMAP_H_ */
