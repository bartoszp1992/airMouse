################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/lib/cursor.c \
../Src/lib/espat.c \
../Src/lib/interrupts.c \
../Src/lib/keyboard.c \
../Src/lib/lsm6ds.c 

OBJS += \
./Src/lib/cursor.o \
./Src/lib/espat.o \
./Src/lib/interrupts.o \
./Src/lib/keyboard.o \
./Src/lib/lsm6ds.o 

C_DEPS += \
./Src/lib/cursor.d \
./Src/lib/espat.d \
./Src/lib/interrupts.d \
./Src/lib/keyboard.d \
./Src/lib/lsm6ds.d 


# Each subdirectory must supply rules for building sources it contributes
Src/lib/%.o Src/lib/%.su Src/lib/%.cyclo: ../Src/lib/%.c Src/lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G071xx -c -I../Inc -I../Inc/airmouse -I../Inc/lib -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src-2f-lib

clean-Src-2f-lib:
	-$(RM) ./Src/lib/cursor.cyclo ./Src/lib/cursor.d ./Src/lib/cursor.o ./Src/lib/cursor.su ./Src/lib/espat.cyclo ./Src/lib/espat.d ./Src/lib/espat.o ./Src/lib/espat.su ./Src/lib/interrupts.cyclo ./Src/lib/interrupts.d ./Src/lib/interrupts.o ./Src/lib/interrupts.su ./Src/lib/keyboard.cyclo ./Src/lib/keyboard.d ./Src/lib/keyboard.o ./Src/lib/keyboard.su ./Src/lib/lsm6ds.cyclo ./Src/lib/lsm6ds.d ./Src/lib/lsm6ds.o ./Src/lib/lsm6ds.su

.PHONY: clean-Src-2f-lib

