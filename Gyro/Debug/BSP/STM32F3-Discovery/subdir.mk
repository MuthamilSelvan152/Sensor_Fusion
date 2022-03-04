################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BSP/STM32F3-Discovery/stm32f3_discovery.c \
../BSP/STM32F3-Discovery/stm32f3_discovery_accelerometer.c \
../BSP/STM32F3-Discovery/stm32f3_discovery_gyroscope.c 

C_DEPS += \
./BSP/STM32F3-Discovery/stm32f3_discovery.d \
./BSP/STM32F3-Discovery/stm32f3_discovery_accelerometer.d \
./BSP/STM32F3-Discovery/stm32f3_discovery_gyroscope.d 

OBJS += \
./BSP/STM32F3-Discovery/stm32f3_discovery.o \
./BSP/STM32F3-Discovery/stm32f3_discovery_accelerometer.o \
./BSP/STM32F3-Discovery/stm32f3_discovery_gyroscope.o 


# Each subdirectory must supply rules for building sources it contributes
BSP/STM32F3-Discovery/%.o: ../BSP/STM32F3-Discovery/%.c BSP/STM32F3-Discovery/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303xC -c -I../Core/Inc -I"D:/Workspace/Sensor_Fusion/Sensor_Fusion/Gyro/BSP/STM32F3-Discovery" -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-BSP-2f-STM32F3-2d-Discovery

clean-BSP-2f-STM32F3-2d-Discovery:
	-$(RM) ./BSP/STM32F3-Discovery/stm32f3_discovery.d ./BSP/STM32F3-Discovery/stm32f3_discovery.o ./BSP/STM32F3-Discovery/stm32f3_discovery_accelerometer.d ./BSP/STM32F3-Discovery/stm32f3_discovery_accelerometer.o ./BSP/STM32F3-Discovery/stm32f3_discovery_gyroscope.d ./BSP/STM32F3-Discovery/stm32f3_discovery_gyroscope.o

.PHONY: clean-BSP-2f-STM32F3-2d-Discovery

