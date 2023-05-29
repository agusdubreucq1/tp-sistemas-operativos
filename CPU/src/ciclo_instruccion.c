#include "ciclo_instruccion.h"

void comenzar_ciclo_instruccion() {
	log_info(cpu_logger,"Comenzar ejecucion del proceso: %u.", contexto_de_ejecucion->pid);

	int salida = 1;

	while(salida){
		t_instruccion* instruccion = fetch_instruccion();
		salida = ejecutar_instruccion(instruccion);
	}
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

		case MOV_IN:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s %s", contexto_de_ejecucion->pid,
																   codigo_instruccion_string(instruccion->codigo_instruccion),
																   instruccion->parametro[0],
																   instruccion->parametro[1]);
			char mensaje_mov_in[30] = "MOV_IN ";
			strcat(mensaje_mov_in, instruccion->parametro[0]);
			strcat(mensaje_mov_in, " ");
			strcat(mensaje_mov_in, instruccion->parametro[1]);
			enviar_mensaje(mensaje_mov_in, socket_memoria);
			break;

		case MOV_OUT:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s %s", contexto_de_ejecucion->pid,
																   codigo_instruccion_string(instruccion->codigo_instruccion),
																   instruccion->parametro[0],
																   instruccion->parametro[1]);
			char mensaje_mov_out[30] = "MOV_OUT ";
			strcat(mensaje_mov_out, instruccion->parametro[0]);
			strcat(mensaje_mov_out, " ");
			strcat(mensaje_mov_out, instruccion->parametro[1]);
			enviar_mensaje(mensaje_mov_out, socket_memoria);
			break;

		case I_O:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s", contexto_de_ejecucion->pid,
																codigo_instruccion_string(instruccion->codigo_instruccion),
																instruccion->parametro[0]);

			char mensaje_io[30] = "I_O ";
			strcat(mensaje_io, instruccion->parametro[0]);
			enviarContexto(mensaje_io);
			salida = 0;
			break;

		case F_OPEN:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s", contexto_de_ejecucion->pid,
																codigo_instruccion_string(instruccion->codigo_instruccion),
																instruccion->parametro[0]);
			char mensaje_f_open[30] = "F_OPEN ";
			strcat(mensaje_f_open, instruccion->parametro[0]);
			enviar_mensaje(mensaje_f_open, socket_Kernel);
			break;

		case F_CLOSE:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s", contexto_de_ejecucion->pid,
																codigo_instruccion_string(instruccion->codigo_instruccion),
																instruccion->parametro[0]);
			char mensaje_f_close[30] = "F_CLOSE ";
			strcat(mensaje_f_close, instruccion->parametro[0]);
			enviar_mensaje(mensaje_f_close, socket_Kernel);
			break;

		case F_SEEK:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s %s",  contexto_de_ejecucion->pid,
																	codigo_instruccion_string(instruccion->codigo_instruccion),
																	instruccion->parametro[0],
																	instruccion->parametro[1]);
			char mensaje_f_seek[30] = "F_SEEK ";
			strcat(mensaje_f_seek, instruccion->parametro[0]);
			strcat(mensaje_f_seek, " ");
			strcat(mensaje_f_seek, instruccion->parametro[1]);
			enviar_mensaje(mensaje_f_seek, socket_Kernel);
			break;

		case F_READ:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s %s %s",  contexto_de_ejecucion->pid,
																	   codigo_instruccion_string(instruccion->codigo_instruccion),
																	   instruccion->parametro[0],
																	   instruccion->parametro[1],
																	   instruccion->parametro[2]);
			char mensaje_f_read[30] = "F_READ ";
			strcat(mensaje_f_read, instruccion->parametro[0]);
			strcat(mensaje_f_read, " ");
			strcat(mensaje_f_read, instruccion->parametro[1]);
			strcat(mensaje_f_read, " ");
			strcat(mensaje_f_read, instruccion->parametro[2]);
			enviar_mensaje(mensaje_f_read, socket_Kernel);
			break;

		case F_WRITE:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s %s %s",  contexto_de_ejecucion->pid,
																	   codigo_instruccion_string(instruccion->codigo_instruccion),
																	   instruccion->parametro[0],
																	   instruccion->parametro[1],
																	   instruccion->parametro[2]);
			char mensaje_f_write[30] = "F_WRITE ";
			strcat(mensaje_f_write, instruccion->parametro[0]);
			strcat(mensaje_f_write, " ");
			strcat(mensaje_f_write, instruccion->parametro[1]);
			strcat(mensaje_f_write, " ");
			strcat(mensaje_f_write, instruccion->parametro[2]);
			enviar_mensaje(mensaje_f_write, socket_Kernel);
			break;

		case F_TRUNCATE:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s %s",  contexto_de_ejecucion->pid,
																	codigo_instruccion_string(instruccion->codigo_instruccion),
																	instruccion->parametro[0],
																	instruccion->parametro[1]);
			char mensaje_f_truncate[30] = "F_TRUNCATE ";
			strcat(mensaje_f_truncate, instruccion->parametro[0]);
			strcat(mensaje_f_truncate, " ");
			strcat(mensaje_f_truncate, instruccion->parametro[1]);
			enviar_mensaje(mensaje_f_truncate, socket_Kernel);
			break;

		case WAIT:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s", contexto_de_ejecucion->pid,
																codigo_instruccion_string(instruccion->codigo_instruccion),
																instruccion->parametro[0]);

			char str[30] = "WAIT ";
			strcat(str, instruccion->parametro[0]);
			enviarContexto(str);
			salida = 0;
			break;

		case SIGNAL:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s", contexto_de_ejecucion->pid,
																codigo_instruccion_string(instruccion->codigo_instruccion),
																instruccion->parametro[0]);

			char str1[30] = "SIGNAL ";
			strcat(str1, instruccion->parametro[0]);
			enviarContexto(str1);
			salida = 0;
			break;

		case CREATE_SEGMENT:
					log_info(cpu_logger, "PID: %u - Ejecutando: %s %s %s",  contexto_de_ejecucion->pid,
																			codigo_instruccion_string(instruccion->codigo_instruccion),
																			instruccion->parametro[0],
																			instruccion->parametro[1]);
					char mensaje_create_segment[30] = "CREATE_SEGMENT ";
					strcat(mensaje_create_segment, instruccion->parametro[0]);
					strcat(mensaje_create_segment, " ");
					strcat(mensaje_create_segment, instruccion->parametro[1]);
					enviar_mensaje(mensaje_create_segment, socket_Kernel);
					break;

		case DELETE_SEGMENT:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s", contexto_de_ejecucion->pid,
																codigo_instruccion_string(instruccion->codigo_instruccion),
																instruccion->parametro[0]);
			char mensaje_delete_segment[30] = "DELETE_SEGMENT ";
			strcat(mensaje_delete_segment, instruccion->parametro[0]);
			enviar_mensaje(mensaje_delete_segment, socket_Kernel);
			break;

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


