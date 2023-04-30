#include "ciclo_instruccion.h"


void comenzar_ciclo_instruccion(t_pcb* pcb, int conexion_kernel) {
	log_trace(cpu_logger, "Comienza ciclo de instrucción para ejecución de proceso %u.", pcb->pid);

    t_instruccion* instruccion_auxilar = siguiente_instruccion(pcb);
    leer_instruccion_y_ejecutar_pcb(instruccion_auxilar, pcb, conexion_kernel);
}

t_instruccion* siguiente_instruccion(t_pcb* pcb) {
    log_info(cpu_logger, "Iniciamos etapa Fetch");

    t_instruccion* instruccion_auxiliar = list_get(pcb->instrucciones, pcb->program_counter);
    pcb->program_counter += 1;

    return instruccion_auxiliar;
}

void preparar_pcb_para_envio(t_pcb *pcb) {
	liberar_registros(pcb->registros_cpu);
	pcb->registros_cpu = registros_cpu;
	imprimir_registros_por_pantalla(registros_cpu);
}

void leer_instruccion_y_ejecutar_pcb(t_instruccion* instruccion_auxiliar, t_pcb* pcb, int conexion_kernel) {
    log_info(cpu_logger, "Iniciamos etapa Decode");

    switch(instruccion_auxiliar->cod_instruccion) {
    	case SET:
            log_info(cpu_logger, "Iniciamos etapa Execute");
    		log_info(cpu_logger, "PID: <%u> - Ejecutando: <SET> - <%s> - <%s>", pcb->pid,
    				instruccion_auxiliar->parametro[0], instruccion_auxiliar->parametro[1], instruccion_auxiliar->parametro[2]);
            usleep(retardo_instruccion * 1000);
            registros_put(registros_cpu, instruccion_auxiliar->parametro[0], instruccion_auxiliar->parametro[1]);
        	imprimir_registros_por_pantalla(registros_cpu);

    		break;

        case EXITT:
            log_info(cpu_logger, "Iniciamos etapa Execute");
    		log_info(cpu_logger, "PID: <%u> - Ejecutando: <EXIT> - <%s> - <%s>", pcb->id,
    				instruccion_auxiliar->parametro[0], instruccion_auxiliar->parametro[1], instruccion_auxiliar->parametro[2]);

        	pcb->estado = EXITT;

        	preparar_pcb_para_envio(pcb);

        	//enviar_pcb(pcb, conexion_kernel, cpu_logger, TERMINO_PCB);
            //destruir_pcb(pcb);

        	iniciar_registros(&registros_cpu); //Vuelve a inicializar la variable externa en cero
        	log_trace(cpu_logger, "Proceso Terminado en CPU.");

            //resetear_proceso_a_interrumpir();

        	return;
            break;

        default:
            log_error(cpu_logger, "Instruccion inválida");
    }

}
