#ifndef CONFIG
#define CONFIG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>

extern char* ip_memoria;
extern char* puerto_interrupt;
extern char* puerto_dispatch;

// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------
	extern t_log* cpu_logger;

// ------------------------------------------------------------------------------------------
// -- Variables del archivo de configuración --
// ------------------------------------------------------------------------------------------

	extern u_int32_t tam_max_segmentos, retardo_instruccion;
	extern char *ip_memoria, *puerto_memoria, *puerto_escucha, *reemplazo_tlb, *puerto_memoria;


// --------------------------------------

t_config* iniciar_config(char*);

void leer_configs(t_config*);
void imprimir_configs();

#endif
