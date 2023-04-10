#ifndef CONFIG
#define CONFIG

#include "logger.h"


t_config* iniciar_config(char* ruta_config);
void leer_configs(t_log*, t_config*);
void loggear_valores_config(t_log*);


#endif /* CONFIG_H_ */
