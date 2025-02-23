################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../commands.c \
../main.c \
../processes.c \
../signals.c 

C_DEPS += \
./commands.d \
./main.d \
./processes.d \
./signals.d 

OBJS += \
./commands.o \
./main.o \
./processes.o \
./signals.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean--2e-

clean--2e-:
	-$(RM) ./commands.d ./commands.o ./main.d ./main.o ./processes.d ./processes.o ./signals.d ./signals.o

.PHONY: clean--2e-

