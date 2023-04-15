################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/client.c \
../src/config.c \
../src/cpu.c \
../src/logger.c \
../src/server.c 

C_DEPS += \
./src/client.d \
./src/config.d \
./src/cpu.d \
./src/logger.d \
./src/server.d 

OBJS += \
./src/client.o \
./src/config.o \
./src/cpu.o \
./src/logger.o \
./src/server.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/client.d ./src/client.o ./src/config.d ./src/config.o ./src/cpu.d ./src/cpu.o ./src/logger.d ./src/logger.o ./src/server.d ./src/server.o

.PHONY: clean-src

