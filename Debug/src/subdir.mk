################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/sock_client.c \
../src/targetinfo.c \
../src/usb_monitor.c 

OBJS += \
./src/sock_client.o \
./src/targetinfo.o \
./src/usb_monitor.o 

C_DEPS += \
./src/sock_client.d \
./src/targetinfo.d \
./src/usb_monitor.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


