#ifndef CICLO_INSTRUCCION_H_
#define CICLO_INSTRUCCION_H_

#include "cpu.h"
#include <utils/pcb.h>


#include <stdio.h>
#include <stdbool.h>
#include <commons/log.h>
#include <utils/datos.h>


void comenzar_ciclo_instruccion(t_pcb*, int);
t_instruccion* siguiente_instruccion(t_pcb*);
int chequear_si_hay_interrupcion_para_atender();
void leer_instruccion_y_ejecutar_pcb(t_instruccion*, t_pcb*, int);


#endif /* CICLO_DE_INSTRUCCION_H_ */
