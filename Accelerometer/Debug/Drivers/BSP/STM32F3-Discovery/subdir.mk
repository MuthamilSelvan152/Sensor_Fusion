################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/STM32F3-Discovery/stm32f3_discovery.c \
../Drivers/BSP/STM32F3-Discovery/stm32f3_discovery_accelerometer.c 

C_DEPS += \
./Drivers/BSP/STM32F3-Discovery/stm32f3_discovery.d \
./Drivers/BSP/STM32F3-Discovery/stm32f3_discovery_accelerometer.d 

OBJS += \
./Drivers/BSP/STM32F3-Discovery/stm32f3_discovery.o \
./Drivers/BSP/STM32F3-Discovery/stm32f3_discovery_accelerometer.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32F3-Discovery/%.o: ../Drivers/BSP/STM32F3-Discovery/%.c Drivers/BSP/STM32F3-Discovery/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303xC -c -I../Core/Inc -I"D:/Workspace/Sensor_Fusion/Accelerometer/Drivers/BSP/STM32F3-Discovery" -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

