################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/LwIP/api_lib.c \
../src/LwIP/api_msg.c \
../src/LwIP/autoip.c \
../src/LwIP/def.c \
../src/LwIP/dhcp.c \
../src/LwIP/dns.c \
../src/LwIP/err.c \
../src/LwIP/etharp.c \
../src/LwIP/ethernetif.c \
../src/LwIP/icmp.c \
../src/LwIP/igmp.c \
../src/LwIP/inet.c \
../src/LwIP/inet_chksum.c \
../src/LwIP/init.c \
../src/LwIP/ip.c \
../src/LwIP/ip_addr.c \
../src/LwIP/ip_frag.c \
../src/LwIP/mem.c \
../src/LwIP/memp.c \
../src/LwIP/netbuf.c \
../src/LwIP/netdb.c \
../src/LwIP/netif.c \
../src/LwIP/netifapi.c \
../src/LwIP/pbuf.c \
../src/LwIP/raw.c \
../src/LwIP/sockets.c \
../src/LwIP/stats.c \
../src/LwIP/sys.c \
../src/LwIP/tcp.c \
../src/LwIP/tcp_in.c \
../src/LwIP/tcp_out.c \
../src/LwIP/tcpip.c \
../src/LwIP/timers.c \
../src/LwIP/udp.c 

OBJS += \
./src/LwIP/api_lib.o \
./src/LwIP/api_msg.o \
./src/LwIP/autoip.o \
./src/LwIP/def.o \
./src/LwIP/dhcp.o \
./src/LwIP/dns.o \
./src/LwIP/err.o \
./src/LwIP/etharp.o \
./src/LwIP/ethernetif.o \
./src/LwIP/icmp.o \
./src/LwIP/igmp.o \
./src/LwIP/inet.o \
./src/LwIP/inet_chksum.o \
./src/LwIP/init.o \
./src/LwIP/ip.o \
./src/LwIP/ip_addr.o \
./src/LwIP/ip_frag.o \
./src/LwIP/mem.o \
./src/LwIP/memp.o \
./src/LwIP/netbuf.o \
./src/LwIP/netdb.o \
./src/LwIP/netif.o \
./src/LwIP/netifapi.o \
./src/LwIP/pbuf.o \
./src/LwIP/raw.o \
./src/LwIP/sockets.o \
./src/LwIP/stats.o \
./src/LwIP/sys.o \
./src/LwIP/tcp.o \
./src/LwIP/tcp_in.o \
./src/LwIP/tcp_out.o \
./src/LwIP/tcpip.o \
./src/LwIP/timers.o \
./src/LwIP/udp.o 

C_DEPS += \
./src/LwIP/api_lib.d \
./src/LwIP/api_msg.d \
./src/LwIP/autoip.d \
./src/LwIP/def.d \
./src/LwIP/dhcp.d \
./src/LwIP/dns.d \
./src/LwIP/err.d \
./src/LwIP/etharp.d \
./src/LwIP/ethernetif.d \
./src/LwIP/icmp.d \
./src/LwIP/igmp.d \
./src/LwIP/inet.d \
./src/LwIP/inet_chksum.d \
./src/LwIP/init.d \
./src/LwIP/ip.d \
./src/LwIP/ip_addr.d \
./src/LwIP/ip_frag.d \
./src/LwIP/mem.d \
./src/LwIP/memp.d \
./src/LwIP/netbuf.d \
./src/LwIP/netdb.d \
./src/LwIP/netif.d \
./src/LwIP/netifapi.d \
./src/LwIP/pbuf.d \
./src/LwIP/raw.d \
./src/LwIP/sockets.d \
./src/LwIP/stats.d \
./src/LwIP/sys.d \
./src/LwIP/tcp.d \
./src/LwIP/tcp_in.d \
./src/LwIP/tcp_out.d \
./src/LwIP/tcpip.d \
./src/LwIP/timers.d \
./src/LwIP/udp.d 


# Each subdirectory must supply rules for building sources it contributes
src/LwIP/%.o: ../src/LwIP/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4" -I../include/ETH_Driver -I../include/lwip -I../include/port -I../include/Modbus -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


