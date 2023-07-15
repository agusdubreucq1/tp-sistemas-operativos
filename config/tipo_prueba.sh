#!/bin/bash

# Cambia esta variable según corresponda en tu sistema
TESTS_DIR=../config

# Pide al usuario que ingrese el tipo de prueba
read -p "Ingresa el tipo de prueba: " test_type

# Verifica si la carpeta de pruebas existe
if [ ! -d "$TESTS_DIR/$test_type" ]; then
    echo "La carpeta de pruebas $test_type no existe"
    exit 1
fi

# Elimina los archivos .config actuales en la carpeta config
rm ./*.config

# Copia los archivos de configuración a la carpeta config
cp "$TESTS_DIR/$test_type"/*.config .

echo "Los archivos de configuración han sido copiados a la carpeta config"

