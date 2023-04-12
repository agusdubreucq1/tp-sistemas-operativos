#include "cpu.h"

int main(int argc, char** argv){


    cpu_logger = iniciar_logger("../../logs/logCPU.log", "CPU");
    cpu_config = iniciar_config("../../config/CPU.config");

    leer_configs(cpu_config, cpu_logger);
    log_info(cpu_logger, "¡CPU iniciado correctamente!");

	return EXIT_SUCCESS;
}
