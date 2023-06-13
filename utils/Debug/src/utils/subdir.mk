################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/cliente.c \
../src/utils/deserializar.c \
../src/utils/instruccion.c \
../src/utils/pcb.c \
../src/utils/recurso.c \
../src/utils/serializar.c \
../src/utils/servidor.c \
../src/utils/utils.c 

C_DEPS += \
./src/utils/cliente.d \
./src/utils/deserializar.d \
./src/utils/instruccion.d \
./src/utils/pcb.d \
./src/utils/recurso.d \
./src/utils/serializar.d \
./src/utils/servidor.d \
./src/utils/utils.d 

OBJS += \
./src/utils/cliente.o \
./src/utils/deserializar.o \
./src/utils/instruccion.o \
./src/utils/pcb.o \
./src/utils/recurso.o \
./src/utils/serializar.o \
./src/utils/servidor.o \
./src/utils/utils.o 


# Each subdirectory must supply rules for building sources it contributes
src/utils/%.o: ../src/utils/%.c src/utils/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-utils

clean-src-2f-utils:
	-$(RM) ./src/utils/cliente.d ./src/utils/cliente.o ./src/utils/deserializar.d ./src/utils/deserializar.o ./src/utils/instruccion.d ./src/utils/instruccion.o ./src/utils/pcb.d ./src/utils/pcb.o ./src/utils/recurso.d ./src/utils/recurso.o ./src/utils/serializar.d ./src/utils/serializar.o ./src/utils/servidor.d ./src/utils/servidor.o ./src/utils/utils.d ./src/utils/utils.o

.PHONY: clean-src-2f-utils

