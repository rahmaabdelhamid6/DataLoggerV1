################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/DataLogger/hal/hal_RTC/src/hal_RTC.c 

OBJS += \
./Core/DataLogger/hal/hal_RTC/src/hal_RTC.o 

C_DEPS += \
./Core/DataLogger/hal/hal_RTC/src/hal_RTC.d 


# Each subdirectory must supply rules for building sources it contributes
Core/DataLogger/hal/hal_RTC/src/%.o Core/DataLogger/hal/hal_RTC/src/%.su Core/DataLogger/hal/hal_RTC/src/%.cyclo: ../Core/DataLogger/hal/hal_RTC/src/%.c Core/DataLogger/hal/hal_RTC/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L412xx -c -I../Core/Inc -I"E:/DL/DataLoggeer_V1.0/Core/DataLogger/hal/hal_Switch/inc" -I"E:/DL/DataLoggeer_V1.0/Core/DataLogger/hal/hal_PC/inc" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I"E:/DL/DataLoggeer_V1.0/Core/DataLogger/app/app_callback/appcb_GPIO/inc" -I"E:/DL/DataLoggeer_V1.0/Core/DataLogger/app/app_callback/appcb_RTC/inc" -I"E:/DL/DataLoggeer_V1.0/Core/DataLogger/app/app_DataLogger/inc" -I"E:/DL/DataLoggeer_V1.0/Core/DataLogger/hal/hal_BME280/inc" -I"E:/DL/DataLoggeer_V1.0/Core/DataLogger/hal/hal_EEPROM/inc" -I"E:/DL/DataLoggeer_V1.0/Core/DataLogger/hal/hal_PC/inc" -I"E:/DL/DataLoggeer_V1.0/Core/DataLogger/hal/hal_RTC/inc" -I"E:/DL/DataLoggeer_V1.0/Core/DataLogger/mcal/mcal_Isr/mcal_GPIO_Isr/inc" -I"E:/DL/DataLoggeer_V1.0/Core/DataLogger/mcal/mcal_Isr/mcal_RTC_Isr/inc" -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-DataLogger-2f-hal-2f-hal_RTC-2f-src

clean-Core-2f-DataLogger-2f-hal-2f-hal_RTC-2f-src:
	-$(RM) ./Core/DataLogger/hal/hal_RTC/src/hal_RTC.cyclo ./Core/DataLogger/hal/hal_RTC/src/hal_RTC.d ./Core/DataLogger/hal/hal_RTC/src/hal_RTC.o ./Core/DataLogger/hal/hal_RTC/src/hal_RTC.su

.PHONY: clean-Core-2f-DataLogger-2f-hal-2f-hal_RTC-2f-src

