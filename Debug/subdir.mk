################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../backEnd.c \
../frontEnd.c \
../lb_main.c \
../prio_queue.c 

OBJS += \
./backEnd.o \
./frontEnd.o \
./lb_main.o \
./prio_queue.o 

C_DEPS += \
./backEnd.d \
./frontEnd.d \
./lb_main.d \
./prio_queue.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C Compiler'
	gcc -std=c99 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


