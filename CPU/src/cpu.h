#include "config.h"

#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <commons/string.h>
#include <commons/log.h>

// ------------------------------------------------------------------------------------------
// -- Logger del proceso --
// ------------------------------------------------------------------------------------------

	t_log* cpu_logger;

// ------------------------------------------------------------------------------------------
// -- Variables del archivo de configuración --
// ------------------------------------------------------------------------------------------

	extern u_int32_t tam_max_segmentos, retardo_instruccion;
	extern char *ip_memoria, *puerto_memoria, *puerto_escucha, *reemplazo_tlb, *puerto_memoria;
