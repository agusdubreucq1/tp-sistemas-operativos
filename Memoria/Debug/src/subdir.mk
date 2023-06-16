################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Memoria.c \
../src/bitmap_memoria.c \
../src/config.c \
../src/espacio_de_memoria.c \
../src/tablas_segmento.c 

C_DEPS += \
./src/Memoria.d \
./src/bitmap_memoria.d \
./src/config.d \
./src/espacio_de_memoria.d \
./src/tablas_segmento.d 

OBJS += \
./src/Memoria.o \
./src/bitmap_memoria.o \
./src/config.o \
./src/espacio_de_memoria.o \
./src/tablas_segmento.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2023-1c-Simple-y-Funciona/utils/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/Memoria.d ./src/Memoria.o ./src/bitmap_memoria.d ./src/bitmap_memoria.o ./src/config.d ./src/config.o ./src/espacio_de_memoria.d ./src/espacio_de_memoria.o ./src/tablas_segmento.d ./src/tablas_segmento.o

.PHONY: clean-src

