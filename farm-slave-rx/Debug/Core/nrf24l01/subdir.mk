################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/nrf24l01/nrf24l01.c 

OBJS += \
./Core/nrf24l01/nrf24l01.o 

C_DEPS += \
./Core/nrf24l01/nrf24l01.d 


# Each subdirectory must supply rules for building sources it contributes
Core/nrf24l01/%.o Core/nrf24l01/%.su Core/nrf24l01/%.cyclo: ../Core/nrf24l01/%.c Core/nrf24l01/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"D:/BaitapHK6/nhungs/slave-rx/farm-slave-rx/Core/dependencies" -I"D:/BaitapHK6/nhungs/slave-rx/farm-slave-rx/Core/Inc" -I"D:/BaitapHK6/nhungs/slave-rx/farm-slave-rx/Core/Src" -I"D:/BaitapHK6/nhungs/slave-rx/farm-slave-rx/Core/nrf24l01" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-nrf24l01

clean-Core-2f-nrf24l01:
	-$(RM) ./Core/nrf24l01/nrf24l01.cyclo ./Core/nrf24l01/nrf24l01.d ./Core/nrf24l01/nrf24l01.o ./Core/nrf24l01/nrf24l01.su

.PHONY: clean-Core-2f-nrf24l01

