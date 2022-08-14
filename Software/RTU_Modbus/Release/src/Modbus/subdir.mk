################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Modbus/mb.c \
../src/Modbus/mbascii.c \
../src/Modbus/mbcrc.c \
../src/Modbus/mbfunccoils.c \
../src/Modbus/mbfuncdiag.c \
../src/Modbus/mbfuncdisc.c \
../src/Modbus/mbfuncholding.c \
../src/Modbus/mbfuncinput.c \
../src/Modbus/mbfuncother.c \
../src/Modbus/mbrtu.c \
../src/Modbus/mbtcp.c \
../src/Modbus/mbutils.c 

OBJS += \
./src/Modbus/mb.o \
./src/Modbus/mbascii.o \
./src/Modbus/mbcrc.o \
./src/Modbus/mbfunccoils.o \
./src/Modbus/mbfuncdiag.o \
./src/Modbus/mbfuncdisc.o \
./src/Modbus/mbfuncholding.o \
./src/Modbus/mbfuncinput.o \
./src/Modbus/mbfuncother.o \
./src/Modbus/mbrtu.o \
./src/Modbus/mbtcp.o \
./src/Modbus/mbutils.o 

C_DEPS += \
./src/Modbus/mb.d \
./src/Modbus/mbascii.d \
./src/Modbus/mbcrc.d \
./src/Modbus/mbfunccoils.d \
./src/Modbus/mbfuncdiag.d \
./src/Modbus/mbfuncdisc.d \
./src/Modbus/mbfuncholding.d \
./src/Modbus/mbfuncinput.d \
./src/Modbus/mbfuncother.d \
./src/Modbus/mbrtu.d \
./src/Modbus/mbtcp.d \
./src/Modbus/mbutils.d 


# Each subdirectory must supply rules for building sources it contributes
src/Modbus/%.o: ../src/Modbus/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4" -I../include/ETH_Driver -I../include/lwip -I../include/port -I../include/Modbus -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


