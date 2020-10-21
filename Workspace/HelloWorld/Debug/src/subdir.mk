################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/syscalls.c \
../src/system_stm32f4xx.c 

OBJS += \
./src/main.o \
./src/syscalls.o \
./src/system_stm32f4xx.o 

C_DEPS += \
./src/main.d \
./src/syscalls.d \
./src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F446xx -DUSE_STDPERIPH_DRIVER -DSTM32F4 -DSTM32F446RETx -DDEBUG -I"/Users/otimofie/Desktop/RTOS/Workspace/HelloWorld/CMSIS/core" -I"/Users/otimofie/Desktop/RTOS/Workspace/HelloWorld/Config" -I"/Users/otimofie/Desktop/RTOS/Workspace/HelloWorld/Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"/Users/otimofie/Desktop/RTOS/Workspace/HelloWorld/Third-Party/FreeRTOS/org/Source/include" -I"/Users/otimofie/Desktop/RTOS/Workspace/HelloWorld/CMSIS/device" -I"/Users/otimofie/Desktop/RTOS/Workspace/HelloWorld/inc" -I"/Users/otimofie/Desktop/RTOS/Workspace/HelloWorld/StdPeriph_Driver/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


