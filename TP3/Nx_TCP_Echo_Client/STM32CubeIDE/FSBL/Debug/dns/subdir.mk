################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/albert/Cours/S-6/ETRS606_SPI/TP3/Nx_TCP_Echo_Client/Middlewares/ST/netxduo/addons/dns/nxd_dns.c 

OBJS += \
./dns/nxd_dns.o 

C_DEPS += \
./dns/nxd_dns.d 


# Each subdirectory must supply rules for building sources it contributes
dns/nxd_dns.o: /Users/albert/Cours/S-6/ETRS606_SPI/TP3/Nx_TCP_Echo_Client/Middlewares/ST/netxduo/addons/dns/nxd_dns.c dns/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m55 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32N657xx -DNX_INCLUDE_USER_DEFINE_FILE -DTX_INCLUDE_USER_DEFINE_FILE -DTX_SINGLE_MODE_SECURE=1 -c -I../../../FSBL/NetXDuo/App -I../../../Middlewares/ST/netxduo/addons/dns -I../../../Middlewares/ST/netxduo/addons/mqtt -I../../../FSBL/NetXDuo/Target -I../../../FSBL/Core/Inc -I../../../FSBL/AZURE_RTOS/App -I../../../Drivers/STM32N6xx_HAL_Driver/Inc -I../../../Drivers/CMSIS/Device/ST/STM32N6xx/Include -I../../../Drivers/STM32N6xx_HAL_Driver/Inc/Legacy -I../../../Drivers/BSP/Components/lan8742 -I../../../Middlewares/ST/netxduo/addons/dhcp -I../../../Middlewares/ST/netxduo/tsn/inc -I../../../Middlewares/ST/netxduo/common/drivers/ethernet -I../../../Middlewares/ST/threadx/common/inc -I../../../Middlewares/ST/netxduo/common/inc -I../../../Middlewares/ST/netxduo/ports/cortex_m55/gnu/inc -I../../../Middlewares/ST/threadx/ports/cortex_m55/gnu/inc -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-dns

clean-dns:
	-$(RM) ./dns/nxd_dns.cyclo ./dns/nxd_dns.d ./dns/nxd_dns.o ./dns/nxd_dns.su

.PHONY: clean-dns

