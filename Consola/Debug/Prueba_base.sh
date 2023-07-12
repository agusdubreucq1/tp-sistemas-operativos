#!/bin/bash

# Ruta y nombre del primer ejecutable
ejecutable1="./Consola"
argumentos1="/home/utnso/tp-2023-1c-Simple-y-Funciona/config/Consola.config /home/utnso/tp-2023-1c-Simple-y-Funciona/pseudocodigo/BASE_1"
argumentos2="/home/utnso/tp-2023-1c-Simple-y-Funciona/config/Consola.config /home/utnso/tp-2023-1c-Simple-y-Funciona/pseudocodigo/BASE_2"

# Ejecutar el primer ejecutable en una nueva instancia de bash con argumentos
echo "Ejecutando el primer ejecutable..."
xterm -e "$ejecutable1 $argumentos1" & sleep 0.2 &

# Ejecutar el segundo ejecutable en una nueva instancia de bash con argumentos
echo "Ejecutando el segundo ejecutable..."
xterm -e "$ejecutable1 $argumentos2" & sleep 0.2 &

# Ejecutar el tercer ejecutable en una nueva instancia de bash con argumentos
echo "Ejecutando el tercer ejecutable..."
xterm -e "$ejecutable1 $argumentos2" & sleep 0.2 &

# Fin del script
echo "Todos los ejecutables han sido iniciados en instancias separada"

#./Consola "/home/utnso/tp-2023-1c-Simple-y-Funciona/config/Consola.config" "/home/utnso/tp-2023-1c-Simple-y-Funciona/pseudocodigo/BASE_1" &
#./Consola "/home/utnso/tp-2023-1c-Simple-y-Funciona/config/Consola.config" "/home/utnso/tp-2023-1c-Simple-y-Funciona/pseudocodigo/BASE_2" &
#./Consola "/home/utnso/tp-2023-1c-Simple-y-Funciona/config/Consola.config" "/home/utnso/tp-2023-1c-Simple-y-Funciona/pseudocodigo/BASE_2"
#wait