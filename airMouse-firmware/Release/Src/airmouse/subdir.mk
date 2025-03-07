################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/airmouse/airmouse.c \
../Src/airmouse/am_keys.c \
../Src/airmouse/am_led.c \
../Src/airmouse/am_lp.c 

OBJS += \
./Src/airmouse/airmouse.o \
./Src/airmouse/am_keys.o \
./Src/airmouse/am_led.o \
./Src/airmouse/am_lp.o 

C_DEPS += \
./Src/airmouse/airmouse.d \
./Src/airmouse/am_keys.d \
./Src/airmouse/am_led.d \
./Src/airmouse/am_lp.d 


# Each subdirectory must supply rules for building sources it contributes
Src/airmouse/%.o Src/airmouse/%.su Src/airmouse/%.cyclo: ../Src/airmouse/%.c Src/airmouse/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -DUSE_HAL_DRIVER -DSTM32G071xx -c -I../Inc -I../Inc/airmouse -I../Inc/lib -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src-2f-airmouse

clean-Src-2f-airmouse:
	-$(RM) ./Src/airmouse/airmouse.cyclo ./Src/airmouse/airmouse.d ./Src/airmouse/airmouse.o ./Src/airmouse/airmouse.su ./Src/airmouse/am_keys.cyclo ./Src/airmouse/am_keys.d ./Src/airmouse/am_keys.o ./Src/airmouse/am_keys.su ./Src/airmouse/am_led.cyclo ./Src/airmouse/am_led.d ./Src/airmouse/am_led.o ./Src/airmouse/am_led.su ./Src/airmouse/am_lp.cyclo ./Src/airmouse/am_lp.d ./Src/airmouse/am_lp.o ./Src/airmouse/am_lp.su

.PHONY: clean-Src-2f-airmouse

