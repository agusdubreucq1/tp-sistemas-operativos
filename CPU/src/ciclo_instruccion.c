#include "ciclo_instruccion.h"

void comenzar_ciclo_instruccion() {
	log_info(cpu_logger,"Comenzar ejecucion del proceso: %u.", contexto_de_ejecucion->pid);

	int salida = 1;

	while(salida){
		t_instruccion* instruccion = fetch_instruccion();
		salida = ejecutar_instruccion(instruccion);
		free(instruccion);
	}
	liberar_contexto(contexto_de_ejecucion);
}

t_instruccion* fetch_instruccion() {
    //printf("Fetch Instruccion");

    t_instruccion* instruccion = parsear_instruccion(list_get(contexto_de_ejecucion->instrucciones, contexto_de_ejecucion->program_counter));
    contexto_de_ejecucion->program_counter += 1;

    return instruccion;
}


int ejecutar_instruccion(t_instruccion* instruccion){
	int salida = 0;
	switch(instruccion->codigo_instruccion) {

		case SET:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s %s", contexto_de_ejecucion->pid,
																   codigo_instruccion_string(instruccion->codigo_instruccion),
																   instruccion->parametro[0],
																   instruccion->parametro[1]);
			usleep(retardo_instruccion*1000);
			registros_put(contexto_de_ejecucion->registros_cpu, instruccion->parametro[0], instruccion->parametro[1]);
			//imprimir_registros(contexto_de_ejecucion->registros_cpu);
			salida = 1;
			break;

		case MOV_IN:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s %s", contexto_de_ejecucion->pid,
																   codigo_instruccion_string(instruccion->codigo_instruccion),
																   instruccion->parametro[0],
																   instruccion->parametro[1]);

			int es_desplazamiento_valido_in = desplazamiento_valido(contexto_de_ejecucion->registros_cpu,
																	instruccion->parametro[0],
															        atoi(instruccion->parametro[1]));

			if(!es_desplazamiento_valido_in){
				log_info(cpu_logger, "PID: %u - Error SEG_FAULT - Segmento: - Offset: - Tamaño: ", contexto_de_ejecucion->pid);
				enviarContexto("MOV_IN SEG_FAULT");
			}
			else {

				char mensaje_mov_in[100] = "";

				int dir_logica = atoi(instruccion->parametro[1]);
				void* dir_fisica = direccion_fisica(dir_logica);

				int tamanio_registro = registros_get_size(contexto_de_ejecucion->registros_cpu, instruccion->parametro[0]);

				sprintf(mensaje_mov_in, "MOV_IN %p %d", dir_fisica, tamanio_registro);//concatena la direccion fisica y cuantos bytes leer

				log_info(cpu_logger, "PID: %u - Accion: LEER - Direccion Fisica: %p - Tamaño:%d - Origen: CPU \n ",
									contexto_de_ejecucion->pid,
									dir_fisica,
									tamanio_registro);

				enviar_mensaje(mensaje_mov_in, socket_memoria);
				recibir_operacion(socket_memoria);

				char *valor_registro_nuevo = recibir_instruccion(socket_memoria, cpu_logger);

				registros_put(contexto_de_ejecucion->registros_cpu, instruccion->parametro[0], valor_registro_nuevo);
				//printf("\nValor del registro que me llega de MOV IN: %s\n\n", valor_registro_nuevo);
				print_registos(contexto_de_ejecucion->registros_cpu);
			}

			salida = 1;
			break;

		case MOV_OUT:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s %s", contexto_de_ejecucion->pid,
																   codigo_instruccion_string(instruccion->codigo_instruccion),
																   instruccion->parametro[0],
																   instruccion->parametro[1]);

			int es_desplazamiento_valido_out = desplazamiento_valido(contexto_de_ejecucion->registros_cpu,
																	 instruccion->parametro[1],
																     atoi(instruccion->parametro[0]));

			if(!es_desplazamiento_valido_out){
				log_info(cpu_logger, "PID: %u - Error SEG_FAULT - Segmento: - Offset: - Tamaño: ",contexto_de_ejecucion->pid);
				enviarContexto("MOV_OUT SEG_FAULT");
			}
			else {

				char mensaje_mov_out[100] = "";

				int direccion_logica = atoi(instruccion->parametro[0]);
				void* dir_fisica = direccion_fisica(direccion_logica);
				char* valor_registro;
				valor_registro = registros_get_value(contexto_de_ejecucion->registros_cpu, instruccion->parametro[1]);
				int tamanio_registro = registros_get_size(contexto_de_ejecucion->registros_cpu, instruccion->parametro[1]);
				valor_registro[tamanio_registro]='\0';

				sprintf(mensaje_mov_out, "MOV_OUT %p %s %d", dir_fisica, valor_registro, tamanio_registro);//concatena la direccion fisica, el valor y cuantos bytes escribir

				log_info(cpu_logger, "PID: %u - Accion: ESCRIBIR - Direccion Fisica: %p - Tamaño: %d - Origen: CPU\n ",
									contexto_de_ejecucion->pid,
									dir_fisica,//para castearlo a void*
									tamanio_registro);

				enviar_mensaje(mensaje_mov_out, socket_memoria);

			}

			salida = 1;
			break;

		case I_O:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s", contexto_de_ejecucion->pid,
																codigo_instruccion_string(instruccion->codigo_instruccion),
																instruccion->parametro[0]);

			char mensaje_io[30] = "I_O ";
			concatenar_mensaje_con_1_parametro(mensaje_io, instruccion);
			enviarContexto(mensaje_io);
			break;

		case F_OPEN:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s", contexto_de_ejecucion->pid,
																codigo_instruccion_string(instruccion->codigo_instruccion),
																instruccion->parametro[0]);
			char mensaje_f_open[30] = "F_OPEN ";
			concatenar_mensaje_con_1_parametro(mensaje_f_open, instruccion);
			enviarContexto(mensaje_f_open);

			break;

		case F_CLOSE:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s", contexto_de_ejecucion->pid,
																codigo_instruccion_string(instruccion->codigo_instruccion),
																instruccion->parametro[0]);
			char mensaje_f_close[30] = "F_CLOSE ";
			concatenar_mensaje_con_1_parametro(mensaje_f_close, instruccion);
			enviarContexto(mensaje_f_close);
			break;

		case F_SEEK:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s %s",  contexto_de_ejecucion->pid,
																	codigo_instruccion_string(instruccion->codigo_instruccion),
																	instruccion->parametro[0],
																	instruccion->parametro[1]);
			char mensaje_f_seek[30] = "F_SEEK ";
			concatenar_mensaje_con_2_parametros(mensaje_f_seek, instruccion);
			enviarContexto(mensaje_f_seek);
			break;

		case F_READ:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s %s %s",  contexto_de_ejecucion->pid,
																	   codigo_instruccion_string(instruccion->codigo_instruccion),
																	   instruccion->parametro[0],
																	   instruccion->parametro[1],
																	   instruccion->parametro[2]);
			char mensaje_f_read[100] = "";
			int dir_logica = atoi(instruccion->parametro[1]);
			void* dir_fisica_read = direccion_fisica(dir_logica);
			int tamanio = atoi(instruccion->parametro[2]);
			sprintf(mensaje_f_read, "F_READ %s %p %d", instruccion->parametro[0], dir_fisica_read, tamanio);
			//concatenar_mensaje_con_3_parametros(mensaje_f_read, instruccion);
			enviarContexto(mensaje_f_read);
			break;

		case F_WRITE:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s %s %s",  contexto_de_ejecucion->pid,
																	   codigo_instruccion_string(instruccion->codigo_instruccion),
																	   instruccion->parametro[0],
																	   instruccion->parametro[1],
																	   instruccion->parametro[2]);
			char mensaje_f_write[100] = "";
			int dir_logica_write = atoi(instruccion->parametro[1]);
			//printf("\ndir_logica: %d\n\n", dir_logica_write);
			void* dir_fisica_write = direccion_fisica(dir_logica_write);
			//printf("\ndir_fisica: %p\n\n", dir_fisica_write);
			int tamanio_write = atoi(instruccion->parametro[2]);
			sprintf(mensaje_f_write, "F_WRITE %s %p %d", instruccion->parametro[0], dir_fisica_write, tamanio_write);
			//concatenar_mensaje_con_3_parametros(mensaje_f_read, instruccion);
			enviarContexto(mensaje_f_write);
			break;

		case F_TRUNCATE:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s %s",  contexto_de_ejecucion->pid,
																	codigo_instruccion_string(instruccion->codigo_instruccion),
																	instruccion->parametro[0],
																	instruccion->parametro[1]);
			char mensaje_f_truncate[30] = "F_TRUNCATE ";

			concatenar_mensaje_con_2_parametros(mensaje_f_truncate, instruccion);

			enviarContexto(mensaje_f_truncate);
			break;

		case WAIT:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s", contexto_de_ejecucion->pid,
																codigo_instruccion_string(instruccion->codigo_instruccion),
																instruccion->parametro[0]);

			char str[30] = "WAIT ";
			concatenar_mensaje_con_1_parametro(str, instruccion);
			enviarContexto(str);
			break;

		case SIGNAL:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s", contexto_de_ejecucion->pid,
																codigo_instruccion_string(instruccion->codigo_instruccion),
																instruccion->parametro[0]);

			char str1[30] = "SIGNAL ";
			concatenar_mensaje_con_1_parametro(str1, instruccion);
			enviarContexto(str1);
			break;

		case CREATE_SEGMENT:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s %s",  contexto_de_ejecucion->pid,
																	codigo_instruccion_string(instruccion->codigo_instruccion),
																	instruccion->parametro[0],
																	instruccion->parametro[1]);
			char mensaje_create_segment[30] = "CREATE_SEGMENT ";

			concatenar_mensaje_con_2_parametros(mensaje_create_segment, instruccion);

			if(atoi(instruccion->parametro[1]) > atoi(tam_max_segmento)){
				log_info(cpu_logger, "PID: %u - Error OUT_OF_MEMORY - Segmento: - Offset: - Tamaño: ",contexto_de_ejecucion->pid);
				enviarContexto("CREATE_SEGMENT OUT_OF_MEMORY");
			}else{
			enviarContexto(mensaje_create_segment);
			}
			break;

		case DELETE_SEGMENT:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s %s", contexto_de_ejecucion->pid,
																codigo_instruccion_string(instruccion->codigo_instruccion),
																instruccion->parametro[0]);
			char mensaje_delete_segment[30] = "DELETE_SEGMENT ";

			concatenar_mensaje_con_1_parametro(mensaje_delete_segment, instruccion);

			enviarContexto(mensaje_delete_segment);
			break;

		case YIELD:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s", contexto_de_ejecucion->pid,
														     codigo_instruccion_string(instruccion->codigo_instruccion));
			enviarContexto("YIELD");
			break;

		case EXIT:
			log_info(cpu_logger, "PID: %u - Ejecutando: %s", contexto_de_ejecucion->pid,
														     codigo_instruccion_string(instruccion->codigo_instruccion));
			enviarContexto("EXIT");
			break;

		default:
			log_error(cpu_logger, "Instruccion inválida");
	}

	return salida;
}



void enviarContexto(char* motivo){
	log_trace(cpu_logger, "enviando a kernel: %s", motivo);
	t_paquete* paquete;
	paquete = serializar_contexto(contexto_de_ejecucion);
	agregar_a_paquete(paquete, motivo, strlen(motivo)+1);

	enviar_paquete(paquete, socket_Kernel, cpu_logger, "Kernel");
}

char* concatenar_mensaje_con_3_parametros(char mensaje[30], t_instruccion* instruccion){
	strcat(mensaje, instruccion->parametro[0]);
	strcat(mensaje, " ");
	strcat(mensaje, instruccion->parametro[1]);
	strcat(mensaje, " ");
	strcat(mensaje, instruccion->parametro[2]);
	return mensaje;
}
char* concatenar_mensaje_con_2_parametros(char mensaje[30], t_instruccion* instruccion){
	strcat(mensaje, instruccion->parametro[0]);
	strcat(mensaje, " ");
	strcat(mensaje, instruccion->parametro[1]);
	return mensaje;
}
char* concatenar_mensaje_con_1_parametro(char mensaje[30], t_instruccion* instruccion){
	strcat(mensaje, instruccion->parametro[0]);
	return mensaje;
}

void liberar_contexto(t_contexto_ejecucion* contexto){
	free(contexto->registros_cpu);
	list_destroy_and_destroy_elements(contexto->tabla_segmentos->segmentos, liberar_elemento_list);
	free(contexto->tabla_segmentos);
	list_destroy_and_destroy_elements(contexto->instrucciones, liberar_elemento_list);
	free(contexto);
}

void liberar_elemento_list(void* elemento){
	free(elemento);
}
