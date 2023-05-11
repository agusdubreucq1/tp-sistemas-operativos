#ifndef SERVIDOR_CPU_H_
#define SERVIDOR_CPU_H_

#include "ciclo_instruccion.h"

void terminar_cpu();
void iniciar_servidor_cpu();
void atencion_por_cpu_dispatch(int* cliente_dispatch);
void atencion_por_cpu_interrupt(int* cliente_interrupt);

#endif
