################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bitmap.c \
../src/bloques.c \
../src/config.c \
../src/fcb.c \
../src/fileSystem.c \
../src/superbloque.c 

C_DEPS += \
./src/bitmap.d \
./src/bloques.d \
./src/config.d \
./src/fcb.d \
./src/fileSystem.d \
./src/superbloque.d 

OBJS += \
./src/bitmap.o \
./src/bloques.o \
./src/config.o \
./src/fcb.o \
./src/fileSystem.o \
./src/superbloque.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2023-1c-Simple-y-Funciona/utils/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/bitmap.d ./src/bitmap.o ./src/bloques.d ./src/bloques.o ./src/config.d ./src/config.o ./src/fcb.d ./src/fcb.o ./src/fileSystem.d ./src/fileSystem.o ./src/superbloque.d ./src/superbloque.o

.PHONY: clean-src

