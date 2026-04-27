################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/albert/Cours/S-6/Projet_IA_2.0/Nx_TCP_Echo_Client/FSBL/X-CUBE-AI/App/app_x-cube-ai.c \
/Users/albert/Cours/S-6/Projet_IA_2.0/Nx_TCP_Echo_Client/FSBL/X-CUBE-AI/App/network.c \
/Users/albert/Cours/S-6/Projet_IA_2.0/Nx_TCP_Echo_Client/FSBL/X-CUBE-AI/App/network_data.c \
/Users/albert/Cours/S-6/Projet_IA_2.0/Nx_TCP_Echo_Client/FSBL/X-CUBE-AI/App/network_data_params.c 

OBJS += \
./App/app_x-cube-ai.o \
./App/network.o \
./App/network_data.o \
./App/network_data_params.o 

C_DEPS += \
./App/app_x-cube-ai.d \
./App/network.d \
./App/network_data.d \
./App/network_data_params.d 


# Each subdirectory must supply rules for building sources it contributes
App/app_x-cube-ai.o: /Users/albert/Cours/S-6/Projet_IA_2.0/Nx_TCP_Echo_Client/FSBL/X-CUBE-AI/App/app_x-cube-ai.c App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m55 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32N657xx -DNX_INCLUDE_USER_DEFINE_FILE -DTX_INCLUDE_USER_DEFINE_FILE -DTX_SINGLE_MODE_SECURE=1 -c -I../../../FSBL/NetXDuo/App -I../../../FSBL/NetXDuo/Target -I../../../FSBL/Core/Inc -I../../../FSBL/AZURE_RTOS/App -I../../../Drivers/STM32N6xx_HAL_Driver/Inc -I../../../Drivers/CMSIS/Device/ST/STM32N6xx/Include -I../../../Drivers/STM32N6xx_HAL_Driver/Inc/Legacy -I../../../Drivers/BSP/Components/lan8742 -I../../../Middlewares/ST/netxduo/addons/dhcp -I../../../Middlewares/ST/netxduo/addons/dns -I../../../Middlewares/ST/netxduo/addons/mqtt -I../../../Middlewares/ST/netxduo/addons/sntp -I../../../Middlewares/ST/netxduo/tsn/inc -I../../../Middlewares/ST/netxduo/common/drivers/ethernet -I../../../Middlewares/ST/threadx/common/inc -I../../../Middlewares/ST/netxduo/common/inc -I../../../Middlewares/ST/netxduo/ports/cortex_m55/gnu/inc -I../../../Middlewares/ST/threadx/ports/cortex_m55/gnu/inc -I../../../Drivers/CMSIS/Include -I../../../FSBL/X-CUBE-AI/App -I../../../FSBL/X-CUBE-AI -I../../../Middlewares/ST/AI/Inc -I../../../Drivers/BSP/Components/hts221 -I../../../Drivers/BSP/Components/lps22hh -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
App/network.o: /Users/albert/Cours/S-6/Projet_IA_2.0/Nx_TCP_Echo_Client/FSBL/X-CUBE-AI/App/network.c App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m55 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32N657xx -DNX_INCLUDE_USER_DEFINE_FILE -DTX_INCLUDE_USER_DEFINE_FILE -DTX_SINGLE_MODE_SECURE=1 -c -I../../../FSBL/NetXDuo/App -I../../../FSBL/NetXDuo/Target -I../../../FSBL/Core/Inc -I../../../FSBL/AZURE_RTOS/App -I../../../Drivers/STM32N6xx_HAL_Driver/Inc -I../../../Drivers/CMSIS/Device/ST/STM32N6xx/Include -I../../../Drivers/STM32N6xx_HAL_Driver/Inc/Legacy -I../../../Drivers/BSP/Components/lan8742 -I../../../Middlewares/ST/netxduo/addons/dhcp -I../../../Middlewares/ST/netxduo/addons/dns -I../../../Middlewares/ST/netxduo/addons/mqtt -I../../../Middlewares/ST/netxduo/addons/sntp -I../../../Middlewares/ST/netxduo/tsn/inc -I../../../Middlewares/ST/netxduo/common/drivers/ethernet -I../../../Middlewares/ST/threadx/common/inc -I../../../Middlewares/ST/netxduo/common/inc -I../../../Middlewares/ST/netxduo/ports/cortex_m55/gnu/inc -I../../../Middlewares/ST/threadx/ports/cortex_m55/gnu/inc -I../../../Drivers/CMSIS/Include -I../../../FSBL/X-CUBE-AI/App -I../../../FSBL/X-CUBE-AI -I../../../Middlewares/ST/AI/Inc -I../../../Drivers/BSP/Components/hts221 -I../../../Drivers/BSP/Components/lps22hh -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
App/network_data.o: /Users/albert/Cours/S-6/Projet_IA_2.0/Nx_TCP_Echo_Client/FSBL/X-CUBE-AI/App/network_data.c App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m55 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32N657xx -DNX_INCLUDE_USER_DEFINE_FILE -DTX_INCLUDE_USER_DEFINE_FILE -DTX_SINGLE_MODE_SECURE=1 -c -I../../../FSBL/NetXDuo/App -I../../../FSBL/NetXDuo/Target -I../../../FSBL/Core/Inc -I../../../FSBL/AZURE_RTOS/App -I../../../Drivers/STM32N6xx_HAL_Driver/Inc -I../../../Drivers/CMSIS/Device/ST/STM32N6xx/Include -I../../../Drivers/STM32N6xx_HAL_Driver/Inc/Legacy -I../../../Drivers/BSP/Components/lan8742 -I../../../Middlewares/ST/netxduo/addons/dhcp -I../../../Middlewares/ST/netxduo/addons/dns -I../../../Middlewares/ST/netxduo/addons/mqtt -I../../../Middlewares/ST/netxduo/addons/sntp -I../../../Middlewares/ST/netxduo/tsn/inc -I../../../Middlewares/ST/netxduo/common/drivers/ethernet -I../../../Middlewares/ST/threadx/common/inc -I../../../Middlewares/ST/netxduo/common/inc -I../../../Middlewares/ST/netxduo/ports/cortex_m55/gnu/inc -I../../../Middlewares/ST/threadx/ports/cortex_m55/gnu/inc -I../../../Drivers/CMSIS/Include -I../../../FSBL/X-CUBE-AI/App -I../../../FSBL/X-CUBE-AI -I../../../Middlewares/ST/AI/Inc -I../../../Drivers/BSP/Components/hts221 -I../../../Drivers/BSP/Components/lps22hh -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
App/network_data_params.o: /Users/albert/Cours/S-6/Projet_IA_2.0/Nx_TCP_Echo_Client/FSBL/X-CUBE-AI/App/network_data_params.c App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m55 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32N657xx -DNX_INCLUDE_USER_DEFINE_FILE -DTX_INCLUDE_USER_DEFINE_FILE -DTX_SINGLE_MODE_SECURE=1 -c -I../../../FSBL/NetXDuo/App -I../../../FSBL/NetXDuo/Target -I../../../FSBL/Core/Inc -I../../../FSBL/AZURE_RTOS/App -I../../../Drivers/STM32N6xx_HAL_Driver/Inc -I../../../Drivers/CMSIS/Device/ST/STM32N6xx/Include -I../../../Drivers/STM32N6xx_HAL_Driver/Inc/Legacy -I../../../Drivers/BSP/Components/lan8742 -I../../../Middlewares/ST/netxduo/addons/dhcp -I../../../Middlewares/ST/netxduo/addons/dns -I../../../Middlewares/ST/netxduo/addons/mqtt -I../../../Middlewares/ST/netxduo/addons/sntp -I../../../Middlewares/ST/netxduo/tsn/inc -I../../../Middlewares/ST/netxduo/common/drivers/ethernet -I../../../Middlewares/ST/threadx/common/inc -I../../../Middlewares/ST/netxduo/common/inc -I../../../Middlewares/ST/netxduo/ports/cortex_m55/gnu/inc -I../../../Middlewares/ST/threadx/ports/cortex_m55/gnu/inc -I../../../Drivers/CMSIS/Include -I../../../FSBL/X-CUBE-AI/App -I../../../FSBL/X-CUBE-AI -I../../../Middlewares/ST/AI/Inc -I../../../Drivers/BSP/Components/hts221 -I../../../Drivers/BSP/Components/lps22hh -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App

clean-App:
	-$(RM) ./App/app_x-cube-ai.cyclo ./App/app_x-cube-ai.d ./App/app_x-cube-ai.o ./App/app_x-cube-ai.su ./App/network.cyclo ./App/network.d ./App/network.o ./App/network.su ./App/network_data.cyclo ./App/network_data.d ./App/network_data.o ./App/network_data.su ./App/network_data_params.cyclo ./App/network_data_params.d ./App/network_data_params.o ./App/network_data_params.su

.PHONY: clean-App

