################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/albert/Cours/S-6/ETRS606_SPI/TP2/Nx_TCP_Echo_Client/Nx_TCP_Echo_Client/FSBL/NetXDuo/App/app_netxduo.c \
../Application/User/NetXDuo/App/nxd_dns.c \
../Application/User/NetXDuo/App/nxd_mqtt_client.c 

OBJS += \
./Application/User/NetXDuo/App/app_netxduo.o \
./Application/User/NetXDuo/App/nxd_dns.o \
./Application/User/NetXDuo/App/nxd_mqtt_client.o 

C_DEPS += \
./Application/User/NetXDuo/App/app_netxduo.d \
./Application/User/NetXDuo/App/nxd_dns.d \
./Application/User/NetXDuo/App/nxd_mqtt_client.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/NetXDuo/App/app_netxduo.o: /Users/albert/Cours/S-6/ETRS606_SPI/TP2/Nx_TCP_Echo_Client/Nx_TCP_Echo_Client/FSBL/NetXDuo/App/app_netxduo.c Application/User/NetXDuo/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m55 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32N657xx -DNX_INCLUDE_USER_DEFINE_FILE -DTX_INCLUDE_USER_DEFINE_FILE -DTX_SINGLE_MODE_SECURE=1 -c -I../../../FSBL/NetXDuo/App -I"/Users/albert/Cours/S-6/ETRS606_SPI/TP2/Nx_TCP_Echo_Client/Nx_TCP_Echo_Client/STM32CubeIDE/FSBL/Application/User/Core" -I"/Users/albert/Cours/S-6/ETRS606_SPI/TP2/Nx_TCP_Echo_Client/Nx_TCP_Echo_Client/STM32CubeIDE/FSBL/Middlewares/AI/Inc" -I"/Users/albert/Cours/S-6/ETRS606_SPI/TP2/Nx_TCP_Echo_Client/Nx_TCP_Echo_Client/STM32CubeIDE/FSBL/Middlewares/AI/Npu/Devices/STM32N6XX" -I"/Users/albert/Cours/S-6/ETRS606_SPI/TP2/Nx_TCP_Echo_Client/Nx_TCP_Echo_Client/STM32CubeIDE/FSBL/Middlewares/AI/Npu/ll_aton" -I"/Users/albert/Cours/S-6/ETRS606_SPI/TP2/Nx_TCP_Echo_Client/Nx_TCP_Echo_Client/STM32CubeIDE/FSBL/Middlewares/AI/Npu" -I../../../FSBL/NetXDuo/Target -I../../../FSBL/Core/Inc -I../../../FSBL/AZURE_RTOS/App -I../../../Drivers/STM32N6xx_HAL_Driver/Inc -I../../../Drivers/CMSIS/Device/ST/STM32N6xx/Include -I../../../Drivers/STM32N6xx_HAL_Driver/Inc/Legacy -I../../../Drivers/BSP/Components/lan8742 -I../../../Middlewares/ST/netxduo/addons/dhcp -I../../../Middlewares/ST/netxduo/tsn/inc -I../../../Middlewares/ST/netxduo/common/drivers/ethernet -I../../../Middlewares/ST/threadx/common/inc -I../../../Middlewares/ST/netxduo/common/inc -I../../../Middlewares/ST/netxduo/ports/cortex_m55/gnu/inc -I../../../Middlewares/ST/threadx/ports/cortex_m55/gnu/inc -I../../../Drivers/CMSIS/Include -I../../../Middlewares/ST/netxduo/addons/dns -I../../../Middlewares/ST/netxduo/addons/mqtt -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/NetXDuo/App/%.o Application/User/NetXDuo/App/%.su Application/User/NetXDuo/App/%.cyclo: ../Application/User/NetXDuo/App/%.c Application/User/NetXDuo/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m55 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32N657xx -DNX_INCLUDE_USER_DEFINE_FILE -DTX_INCLUDE_USER_DEFINE_FILE -DTX_SINGLE_MODE_SECURE=1 -c -I../../../FSBL/NetXDuo/App -I"/Users/albert/Cours/S-6/ETRS606_SPI/TP2/Nx_TCP_Echo_Client/Nx_TCP_Echo_Client/STM32CubeIDE/FSBL/Application/User/Core" -I"/Users/albert/Cours/S-6/ETRS606_SPI/TP2/Nx_TCP_Echo_Client/Nx_TCP_Echo_Client/STM32CubeIDE/FSBL/Middlewares/AI/Inc" -I"/Users/albert/Cours/S-6/ETRS606_SPI/TP2/Nx_TCP_Echo_Client/Nx_TCP_Echo_Client/STM32CubeIDE/FSBL/Middlewares/AI/Npu/Devices/STM32N6XX" -I"/Users/albert/Cours/S-6/ETRS606_SPI/TP2/Nx_TCP_Echo_Client/Nx_TCP_Echo_Client/STM32CubeIDE/FSBL/Middlewares/AI/Npu/ll_aton" -I"/Users/albert/Cours/S-6/ETRS606_SPI/TP2/Nx_TCP_Echo_Client/Nx_TCP_Echo_Client/STM32CubeIDE/FSBL/Middlewares/AI/Npu" -I../../../FSBL/NetXDuo/Target -I../../../FSBL/Core/Inc -I../../../FSBL/AZURE_RTOS/App -I../../../Drivers/STM32N6xx_HAL_Driver/Inc -I../../../Drivers/CMSIS/Device/ST/STM32N6xx/Include -I../../../Drivers/STM32N6xx_HAL_Driver/Inc/Legacy -I../../../Drivers/BSP/Components/lan8742 -I../../../Middlewares/ST/netxduo/addons/dhcp -I../../../Middlewares/ST/netxduo/tsn/inc -I../../../Middlewares/ST/netxduo/common/drivers/ethernet -I../../../Middlewares/ST/threadx/common/inc -I../../../Middlewares/ST/netxduo/common/inc -I../../../Middlewares/ST/netxduo/ports/cortex_m55/gnu/inc -I../../../Middlewares/ST/threadx/ports/cortex_m55/gnu/inc -I../../../Drivers/CMSIS/Include -I../../../Middlewares/ST/netxduo/addons/dns -I../../../Middlewares/ST/netxduo/addons/mqtt -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User-2f-NetXDuo-2f-App

clean-Application-2f-User-2f-NetXDuo-2f-App:
	-$(RM) ./Application/User/NetXDuo/App/app_netxduo.cyclo ./Application/User/NetXDuo/App/app_netxduo.d ./Application/User/NetXDuo/App/app_netxduo.o ./Application/User/NetXDuo/App/app_netxduo.su ./Application/User/NetXDuo/App/nxd_dns.cyclo ./Application/User/NetXDuo/App/nxd_dns.d ./Application/User/NetXDuo/App/nxd_dns.o ./Application/User/NetXDuo/App/nxd_dns.su ./Application/User/NetXDuo/App/nxd_mqtt_client.cyclo ./Application/User/NetXDuo/App/nxd_mqtt_client.d ./Application/User/NetXDuo/App/nxd_mqtt_client.o ./Application/User/NetXDuo/App/nxd_mqtt_client.su

.PHONY: clean-Application-2f-User-2f-NetXDuo-2f-App

