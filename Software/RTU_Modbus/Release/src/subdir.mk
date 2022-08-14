################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/fs.c \
../src/fsdata.c \
../src/httpd.c \
../src/httpd_cgi_ssi.c \
../src/main.c \
../src/netconf.c \
../src/stm32f4x7_eth_bsp.c \
../src/stm32f4xx_it.c 

OBJS += \
./src/fs.o \
./src/fsdata.o \
./src/httpd.o \
./src/httpd_cgi_ssi.o \
./src/main.o \
./src/netconf.o \
./src/stm32f4x7_eth_bsp.o \
./src/stm32f4xx_it.o 

C_DEPS += \
./src/fs.d \
./src/fsdata.d \
./src/httpd.d \
./src/httpd_cgi_ssi.d \
./src/main.d \
./src/netconf.d \
./src/stm32f4x7_eth_bsp.d \
./src/stm32f4xx_it.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4" -I../include/ETH_Driver -I../include/lwip -I../include/port -I../include/Modbus -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


