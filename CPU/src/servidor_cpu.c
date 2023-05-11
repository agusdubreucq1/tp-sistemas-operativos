#include "servidor_cpu.h"


/*
void iniciar_servidor_cpu(){

	proceso_a_interrumpir = 0;
	iniciar_registros(&registros_cpu);
	imprimir_registros_por_pantalla(registros_cpu);
	pthread_mutex_init(&semaforo_proceso_a_interrumpir, NULL);

	conexion_dispatch = iniciar_servidor(cpu_logger, "CONEXION_DISPATCH", "0.0.0.0" , puerto_dispatch);//Siempre la ip de escucha es la misma 0.0.0.0



	server_cpu = iniciar_servidor(IP_SERVER, puerto_escucha, cpu_logger);
	log_info(cpu_logger, "Servidor listo para recibir al cliente");

	pthread_create(&atender_kernel, NULL, abrirSocketKernel, NULL);
	pthread_create(&conexionMemoria, NULL, conectarMemoria, NULL);

	pthread_detach(conexionMemoria);
	pthread_join(atender_kernel, NULL);

}*/

