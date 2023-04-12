#include <stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>

t_log* iniciar_logger(void);
t_config* iniciar_config(char* configFile);
void leer_config(t_log*, t_config*);
void terminar_programa(t_log*, t_config*);//agregar socket despues
