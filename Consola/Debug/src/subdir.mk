################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/config.c \
../src/consola.c \
../src/logger.c \
../src/parser.c \
../src/socketConsola.c 

C_DEPS += \
./src/config.d \
./src/consola.d \
./src/logger.d \
./src/parser.d \
./src/socketConsola.d 

OBJS += \
./src/config.o \
./src/consola.o \
./src/logger.o \
./src/parser.o \
./src/socketConsola.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/config.d ./src/config.o ./src/consola.d ./src/consola.o ./src/logger.d ./src/logger.o ./src/parser.d ./src/parser.o ./src/socketConsola.d ./src/socketConsola.o

.PHONY: clean-src

