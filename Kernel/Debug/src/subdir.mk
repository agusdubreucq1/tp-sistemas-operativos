################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/config.c \
../src/kernel.c \
../src/planificador_largo_plazo.c \
../src/recursos.c 

C_DEPS += \
./src/config.d \
./src/kernel.d \
./src/planificador_largo_plazo.d \
./src/recursos.d 

OBJS += \
./src/config.o \
./src/kernel.o \
./src/planificador_largo_plazo.o \
./src/recursos.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2023-1c-Simple-y-Funciona/utils/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/config.d ./src/config.o ./src/kernel.d ./src/kernel.o ./src/planificador_largo_plazo.d ./src/planificador_largo_plazo.o ./src/recursos.d ./src/recursos.o

.PHONY: clean-src

