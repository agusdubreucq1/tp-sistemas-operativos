#include "espacio_de_memoria.h"
#include <stdio.h>
#include <stdlib.h>

void* reservar_espacio_memoria(){

	return malloc(atoi(tam_memoria));

}
