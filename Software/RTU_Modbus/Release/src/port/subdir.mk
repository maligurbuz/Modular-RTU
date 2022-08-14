################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/port/portevent.c \
../src/port/portevent_m.c \
../src/port/portother.c \
../src/port/portserial.c \
../src/port/portserial_m.c \
../src/port/porttcp.c \
../src/port/porttimer.c \
../src/port/porttimer_m.c 

OBJS += \
./src/port/portevent.o \
./src/port/portevent_m.o \
./src/port/portother.o \
./src/port/portserial.o \
./src/port/portserial_m.o \
./src/port/porttcp.o \
./src/port/porttimer.o \
./src/port/porttimer_m.o 

C_DEPS += \
./src/port/portevent.d \
./src/port/portevent_m.d \
./src/port/portother.d \
./src/port/portserial.d \
./src/port/portserial_m.d \
./src/port/porttcp.d \
./src/port/porttimer.d \
./src/port/porttimer_m.d 


# Each subdirectory must supply rules for building sources it contributes
src/port/%.o: ../src/port/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4" -I../include/ETH_Driver -I../include/lwip -I../include/port -I../include/Modbus -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


