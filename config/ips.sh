

#!/bin/bash

# Solicita la nueva dirección IP al usuario
echo -n "Ingresa la nueva direccion IP-KERNEL: "
read ip_kernel

echo -n "Ingresa la nueva direccion IP-CPU: "
read ip_cpu

echo -n "Ingresa la nueva direccion IP-MEMORIA: "
read ip_memoria

echo -n "Ingresa la nueva direccion IP-FILESYSTEM: "
read ip_filesystem

# Define la ruta de los archivos a modificar
archivoConsola="Consola.config"
archivoKernel="Kernel.config"
archivoCPU="CPU.config"
archivoMemoria="Memoria.config"
archivoFile="FileSystem.config"

# Usa sed para buscar y reemplazar la dirección IP en los archivos
sed -i "s/^IP_KERNEL=.*/IP_KERNEL=$ip_kernel/" "$archivoConsola"
sed -i "s/^IP_KERNEL=.*/IP_KERNEL=$ip_kernel/" "$archivoKernel"
sed -i "s/^IP_KERNEL=.*/IP_KERNEL=$ip_kernel/" "$archivoCPU"
sed -i "s/^IP_KERNEL=.*/IP_KERNEL=$ip_kernel/" "$archivoMemoria"
sed -i "s/^IP_KERNEL=.*/IP_KERNEL=$ip_kernel/" "$archivoFile"

sed -i "s/^IP_CPU=.*/IP_CPU=$ip_cpu/" "$archivoConsola"
sed -i "s/^IP_CPU=.*/IP_CPU=$ip_cpu/" "$archivoKernel"
sed -i "s/^IP_CPU=.*/IP_CPU=$ip_cpu/" "$archivoCPU"
sed -i "s/^IP_CPU=.*/IP_CPU=$ip_cpu/" "$archivoMemoria"
sed -i "s/^IP_CPU=.*/IP_CPU=$ip_cpu/" "$archivoFile"

sed -i "s/^IP_MEMORIA=.*/IP_MEMORIA=$ip_memoria/" "$archivoConsola"
sed -i "s/^IP_MEMORIA=.*/IP_MEMORIA=$ip_memoria/" "$archivoKernel"
sed -i "s/^IP_MEMORIA=.*/IP_MEMORIA=$ip_memoria/" "$archivoCPU"
sed -i "s/^IP_MEMORIA=.*/IP_MEMORIA=$ip_memoria/" "$archivoMemoria"
sed -i "s/^IP_MEMORIA=.*/IP_MEMORIA=$ip_memoria/" "$archivoFile"

sed -i "s/^IP_FILESYSTEM=.*/IP_FILESYSTEM=$ip_filesystem/" "$archivoConsola"
sed -i "s/^IP_FILESYSTEM=.*/IP_FILESYSTEM=$ip_filesystem/" "$archivoKernel"
sed -i "s/^IP_FILESYSTEM=.*/IP_FILESYSTEM=$ip_filesystem/" "$archivoCPU"
sed -i "s/^IP_FILESYSTEM=.*/IP_FILESYSTEM=$ip_filesystem/" "$archivoMemoria"
sed -i "s/^IP_FILESYSTEM=.*/IP_FILESYSTEM=$ip_filesystem/" "$archivoFile"

# Imprime un mensaje cuando se haya completado la operación
echo "Direcciones IP actualizadas en los archivos de configuración"



