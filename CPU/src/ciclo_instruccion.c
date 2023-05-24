#include "ciclo_instruccion.h"

void comenzar_ciclo_instruccion() {
	log_info(cpu_logger,"Comenzar ejecucion del proceso: %u.", contexto_de_ejecucion->pid);

	int salida = 1;

	while(salida){
		t_instruccion* instruccion = fetch_instruccion();
		salida = ejecutar_instruccion(instruccion);
	}



    //enviar_instruccion("SET perro", socket_Kernel);
}

t_instruccion* fetch_instruccion() {
    printf("Fetch Instruccion");

    t_instruccion* instruccion = parsear_instruccion(list_get(contexto_de_ejecucion->instrucciones, contexto_de_ejecucion->program_counter));
    contexto_de_ejecucion->program_counter += 1;

    return instruccion;
}


int ejecutar_instruccion(t_instruccion* instruccion){

	log_info(cpu_logger, "Instruccion Ejecutada");
	int salida = 1;
	switch(instruccion->codigo_instruccion) {
		case SET:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s %s", contexto_de_ejecucion->pid,
																   codigo_instruccion_string(instruccion->codigo_instruccion),
																   instruccion->parametro[0],
																   instruccion->parametro[1]);

			usleep(retardo_instruccion);
			registros_put(contexto_de_ejecucion->registros_cpu, instruccion->parametro[0], instruccion->parametro[1]);
			imprimir_registros(contexto_de_ejecucion->registros_cpu);

			break;
		case MOV_IN:   		break;
		case MOV_OUT:  		break;
		case I_O:   		break;
		case F_OPEN:   		break;
		case F_CLOSE:  		break;
		case F_SEEK:		break;
		case F_READ:		break;
		case F_WRITE:		break;
		case F_TRUNCATE:	break;
		case WAIT:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s", contexto_de_ejecucion->pid,
																   codigo_instruccion_string(instruccion->codigo_instruccion),
																   instruccion->parametro[0]);

			char* string = "WAIT ";
			strcat(string, instruccion->parametro[0]);
			printf("%s", string);
			enviarContexto(string);
			free(string);
			salida = 0;
			break;
		case SIGNAL:






			break;
		case CREATE_SEGMENT:break;
		case DELETE_SEGMENT:break;
		case YIELD:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s", contexto_de_ejecucion->pid,
														   codigo_instruccion_string(instruccion->codigo_instruccion));
			enviarContexto("YIELD");
			salida = 0;
			break;
		case EXIT:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s", contexto_de_ejecucion->pid,
														   codigo_instruccion_string(instruccion->codigo_instruccion));
			enviarContexto("EXIT");
			salida = 0;
			break;




			/*log_info(cpu_logger, "Iniciamos etapa Execute");
			log_info(cpu_logger, "PID: <%u> - Ejecutando: <EXIT> - <%s> - <%s>", pcb->pid,
					instruccion_auxiliar->parametro[0], instruccion_auxiliar->parametro[1], instruccion_auxiliar->parametro[2]);

			pcb->estado = EXITT;

			preparar_pcb_para_envio(pcb);

			//enviar_pcb(pcb, conexion_kernel, cpu_logger, TERMINO_PCB);
			//destruir_pcb(pcb);

			//iniciar_registros(&registros_cpu); //Vuelve a inicializar la variable externa en cero
			log_trace(cpu_logger, "Proceso Terminado en CPU.");

			//resetear_proceso_a_interrumpir();

			return;*/


		default:
			log_error(cpu_logger, "Instruccion inválida");
	}
	return salida;
}

void enviarContexto(char* motivo){
	t_paquete* paquete;
	paquete = serializar_contexto(contexto_de_ejecucion);
	agregar_a_paquete(paquete, motivo, strlen(motivo)+1);

	//int tamanio_contexto;
	//memcpy(&tamanio_contexto, paquete->buffer->stream, sizeof(int));
	//printf("\n Contexto enviado:\n\n");
	printf("\ntam_enviado: %ld\n", paquete->buffer->size + 2*sizeof(int));
	printf("\EL VALOR ES: %s\n", motivo);

	enviar_paquete(paquete, socket_Kernel, cpu_logger, "cpu");
}


