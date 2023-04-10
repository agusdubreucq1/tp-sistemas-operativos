#include "cpu.h"

int main(int argc, char** argv){
    if (argc != 2) {
        puts("\n No se introdujo la cantidad de argumentos necesarios"
                "\n   * Primer  argumento: <Archivo de configuracion>");
       //         "\n   * Segundo argumento: <Archivo de log>");
        return EXIT_FAILURE;
    }

	t_config* cpu_config = iniciar_config(argv[1]);
    cpu_logger = iniciar_logger("../../logs/logConsola.log", "CPU");

	leer_configs(cpu_config);


	return EXIT_SUCCESS;
}
