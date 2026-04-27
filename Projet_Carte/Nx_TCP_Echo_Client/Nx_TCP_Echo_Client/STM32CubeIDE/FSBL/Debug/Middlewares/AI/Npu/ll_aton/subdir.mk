################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/AI/Npu/ll_aton/ecloader.c \
../Middlewares/AI/Npu/ll_aton/ll_aton.c \
../Middlewares/AI/Npu/ll_aton/ll_aton_cipher.c \
../Middlewares/AI/Npu/ll_aton/ll_aton_dbgtrc.c \
../Middlewares/AI/Npu/ll_aton/ll_aton_debug.c \
../Middlewares/AI/Npu/ll_aton/ll_aton_lib.c \
../Middlewares/AI/Npu/ll_aton/ll_aton_lib_sw_operators.c \
../Middlewares/AI/Npu/ll_aton/ll_aton_osal_threadx.c \
../Middlewares/AI/Npu/ll_aton/ll_aton_profiler.c \
../Middlewares/AI/Npu/ll_aton/ll_aton_reloc_callbacks.c \
../Middlewares/AI/Npu/ll_aton/ll_aton_reloc_network.c \
../Middlewares/AI/Npu/ll_aton/ll_aton_rt_main.c \
../Middlewares/AI/Npu/ll_aton/ll_aton_runtime.c \
../Middlewares/AI/Npu/ll_aton/ll_aton_util.c \
../Middlewares/AI/Npu/ll_aton/ll_sw_float.c \
../Middlewares/AI/Npu/ll_aton/ll_sw_integer.c 

OBJS += \
./Middlewares/AI/Npu/ll_aton/ecloader.o \
./Middlewares/AI/Npu/ll_aton/ll_aton.o \
./Middlewares/AI/Npu/ll_aton/ll_aton_cipher.o \
./Middlewares/AI/Npu/ll_aton/ll_aton_dbgtrc.o \
./Middlewares/AI/Npu/ll_aton/ll_aton_debug.o \
./Middlewares/AI/Npu/ll_aton/ll_aton_lib.o \
./Middlewares/AI/Npu/ll_aton/ll_aton_lib_sw_operators.o \
./Middlewares/AI/Npu/ll_aton/ll_aton_osal_threadx.o \
./Middlewares/AI/Npu/ll_aton/ll_aton_profiler.o \
./Middlewares/AI/Npu/ll_aton/ll_aton_reloc_callbacks.o \
./Middlewares/AI/Npu/ll_aton/ll_aton_reloc_network.o \
./Middlewares/AI/Npu/ll_aton/ll_aton_rt_main.o \
./Middlewares/AI/Npu/ll_aton/ll_aton_runtime.o \
./Middlewares/AI/Npu/ll_aton/ll_aton_util.o \
./Middlewares/AI/Npu/ll_aton/ll_sw_float.o \
./Middlewares/AI/Npu/ll_aton/ll_sw_integer.o 

C_DEPS += \
./Middlewares/AI/Npu/ll_aton/ecloader.d \
./Middlewares/AI/Npu/ll_aton/ll_aton.d \
./Middlewares/AI/Npu/ll_aton/ll_aton_cipher.d \
./Middlewares/AI/Npu/ll_aton/ll_aton_dbgtrc.d \
./Middlewares/AI/Npu/ll_aton/ll_aton_debug.d \
./Middlewares/AI/Npu/ll_aton/ll_aton_lib.d \
./Middlewares/AI/Npu/ll_aton/ll_aton_lib_sw_operators.d \
./Middlewares/AI/Npu/ll_aton/ll_aton_osal_threadx.d \
./Middlewares/AI/Npu/ll_aton/ll_aton_profiler.d \
./Middlewares/AI/Npu/ll_aton/ll_aton_reloc_callbacks.d \
./Middlewares/AI/Npu/ll_aton/ll_aton_reloc_network.d \
./Middlewares/AI/Npu/ll_aton/ll_aton_rt_main.d \
./Middlewares/AI/Npu/ll_aton/ll_aton_runtime.d \
./Middlewares/AI/Npu/ll_aton/ll_aton_util.d \
./Middlewares/AI/Npu/ll_aton/ll_sw_float.d \
./Middlewares/AI/Npu/ll_aton/ll_sw_integer.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/AI/Npu/ll_aton/%.o Middlewares/AI/Npu/ll_aton/%.su Middlewares/AI/Npu/ll_aton/%.cyclo: ../Middlewares/AI/Npu/ll_aton/%.c Middlewares/AI/Npu/ll_aton/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m55 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32N657xx -DNX_INCLUDE_USER_DEFINE_FILE -DTX_INCLUDE_USER_DEFINE_FILE -DTX_SINGLE_MODE_SECURE=1 -c -I../../../FSBL/NetXDuo/App -I"/Users/albert/Cours/S-6/ETRS606_SPI/TP2/Nx_TCP_Echo_Client/Nx_TCP_Echo_Client/STM32CubeIDE/FSBL/Application/User/Core" -I"/Users/albert/Cours/S-6/ETRS606_SPI/TP2/Nx_TCP_Echo_Client/Nx_TCP_Echo_Client/STM32CubeIDE/FSBL/Middlewares/AI/Inc" -I"/Users/albert/Cours/S-6/ETRS606_SPI/TP2/Nx_TCP_Echo_Client/Nx_TCP_Echo_Client/STM32CubeIDE/FSBL/Middlewares/AI/Npu/Devices/STM32N6XX" -I"/Users/albert/Cours/S-6/ETRS606_SPI/TP2/Nx_TCP_Echo_Client/Nx_TCP_Echo_Client/STM32CubeIDE/FSBL/Middlewares/AI/Npu/ll_aton" -I"/Users/albert/Cours/S-6/ETRS606_SPI/TP2/Nx_TCP_Echo_Client/Nx_TCP_Echo_Client/STM32CubeIDE/FSBL/Middlewares/AI/Npu" -I../../../FSBL/NetXDuo/Target -I../../../FSBL/Core/Inc -I../../../FSBL/AZURE_RTOS/App -I../../../Drivers/STM32N6xx_HAL_Driver/Inc -I../../../Drivers/CMSIS/Device/ST/STM32N6xx/Include -I../../../Drivers/STM32N6xx_HAL_Driver/Inc/Legacy -I../../../Drivers/BSP/Components/lan8742 -I../../../Middlewares/ST/netxduo/addons/dhcp -I../../../Middlewares/ST/netxduo/tsn/inc -I../../../Middlewares/ST/netxduo/common/drivers/ethernet -I../../../Middlewares/ST/threadx/common/inc -I../../../Middlewares/ST/netxduo/common/inc -I../../../Middlewares/ST/netxduo/ports/cortex_m55/gnu/inc -I../../../Middlewares/ST/threadx/ports/cortex_m55/gnu/inc -I../../../Drivers/CMSIS/Include -I../../../Middlewares/ST/netxduo/addons/dns -I../../../Middlewares/ST/netxduo/addons/mqtt -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-AI-2f-Npu-2f-ll_aton

clean-Middlewares-2f-AI-2f-Npu-2f-ll_aton:
	-$(RM) ./Middlewares/AI/Npu/ll_aton/ecloader.cyclo ./Middlewares/AI/Npu/ll_aton/ecloader.d ./Middlewares/AI/Npu/ll_aton/ecloader.o ./Middlewares/AI/Npu/ll_aton/ecloader.su ./Middlewares/AI/Npu/ll_aton/ll_aton.cyclo ./Middlewares/AI/Npu/ll_aton/ll_aton.d ./Middlewares/AI/Npu/ll_aton/ll_aton.o ./Middlewares/AI/Npu/ll_aton/ll_aton.su ./Middlewares/AI/Npu/ll_aton/ll_aton_cipher.cyclo ./Middlewares/AI/Npu/ll_aton/ll_aton_cipher.d ./Middlewares/AI/Npu/ll_aton/ll_aton_cipher.o ./Middlewares/AI/Npu/ll_aton/ll_aton_cipher.su ./Middlewares/AI/Npu/ll_aton/ll_aton_dbgtrc.cyclo ./Middlewares/AI/Npu/ll_aton/ll_aton_dbgtrc.d ./Middlewares/AI/Npu/ll_aton/ll_aton_dbgtrc.o ./Middlewares/AI/Npu/ll_aton/ll_aton_dbgtrc.su ./Middlewares/AI/Npu/ll_aton/ll_aton_debug.cyclo ./Middlewares/AI/Npu/ll_aton/ll_aton_debug.d ./Middlewares/AI/Npu/ll_aton/ll_aton_debug.o ./Middlewares/AI/Npu/ll_aton/ll_aton_debug.su ./Middlewares/AI/Npu/ll_aton/ll_aton_lib.cyclo ./Middlewares/AI/Npu/ll_aton/ll_aton_lib.d ./Middlewares/AI/Npu/ll_aton/ll_aton_lib.o ./Middlewares/AI/Npu/ll_aton/ll_aton_lib.su ./Middlewares/AI/Npu/ll_aton/ll_aton_lib_sw_operators.cyclo ./Middlewares/AI/Npu/ll_aton/ll_aton_lib_sw_operators.d ./Middlewares/AI/Npu/ll_aton/ll_aton_lib_sw_operators.o ./Middlewares/AI/Npu/ll_aton/ll_aton_lib_sw_operators.su ./Middlewares/AI/Npu/ll_aton/ll_aton_osal_threadx.cyclo ./Middlewares/AI/Npu/ll_aton/ll_aton_osal_threadx.d ./Middlewares/AI/Npu/ll_aton/ll_aton_osal_threadx.o ./Middlewares/AI/Npu/ll_aton/ll_aton_osal_threadx.su ./Middlewares/AI/Npu/ll_aton/ll_aton_profiler.cyclo ./Middlewares/AI/Npu/ll_aton/ll_aton_profiler.d ./Middlewares/AI/Npu/ll_aton/ll_aton_profiler.o ./Middlewares/AI/Npu/ll_aton/ll_aton_profiler.su ./Middlewares/AI/Npu/ll_aton/ll_aton_reloc_callbacks.cyclo ./Middlewares/AI/Npu/ll_aton/ll_aton_reloc_callbacks.d ./Middlewares/AI/Npu/ll_aton/ll_aton_reloc_callbacks.o ./Middlewares/AI/Npu/ll_aton/ll_aton_reloc_callbacks.su ./Middlewares/AI/Npu/ll_aton/ll_aton_reloc_network.cyclo ./Middlewares/AI/Npu/ll_aton/ll_aton_reloc_network.d ./Middlewares/AI/Npu/ll_aton/ll_aton_reloc_network.o ./Middlewares/AI/Npu/ll_aton/ll_aton_reloc_network.su ./Middlewares/AI/Npu/ll_aton/ll_aton_rt_main.cyclo ./Middlewares/AI/Npu/ll_aton/ll_aton_rt_main.d ./Middlewares/AI/Npu/ll_aton/ll_aton_rt_main.o ./Middlewares/AI/Npu/ll_aton/ll_aton_rt_main.su ./Middlewares/AI/Npu/ll_aton/ll_aton_runtime.cyclo ./Middlewares/AI/Npu/ll_aton/ll_aton_runtime.d ./Middlewares/AI/Npu/ll_aton/ll_aton_runtime.o ./Middlewares/AI/Npu/ll_aton/ll_aton_runtime.su ./Middlewares/AI/Npu/ll_aton/ll_aton_util.cyclo ./Middlewares/AI/Npu/ll_aton/ll_aton_util.d ./Middlewares/AI/Npu/ll_aton/ll_aton_util.o ./Middlewares/AI/Npu/ll_aton/ll_aton_util.su ./Middlewares/AI/Npu/ll_aton/ll_sw_float.cyclo ./Middlewares/AI/Npu/ll_aton/ll_sw_float.d ./Middlewares/AI/Npu/ll_aton/ll_sw_float.o ./Middlewares/AI/Npu/ll_aton/ll_sw_float.su ./Middlewares/AI/Npu/ll_aton/ll_sw_integer.cyclo ./Middlewares/AI/Npu/ll_aton/ll_sw_integer.d ./Middlewares/AI/Npu/ll_aton/ll_sw_integer.o ./Middlewares/AI/Npu/ll_aton/ll_sw_integer.su

.PHONY: clean-Middlewares-2f-AI-2f-Npu-2f-ll_aton

