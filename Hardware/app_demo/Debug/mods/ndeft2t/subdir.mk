################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mods/ndeft2t/ndeft2t.c 

OBJS += \
./mods/ndeft2t/ndeft2t.o 

C_DEPS += \
./mods/ndeft2t/ndeft2t.d 


# Each subdirectory must supply rules for building sources it contributes
mods/ndeft2t/%.o: ../mods/ndeft2t/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=c99 -DDEBUG -D__CODE_RED -DCORE_M0PLUS -D__REDLIB__ -I"C:\Users\kille\Desktop\Senior Project\SeniorProject\Hardware\app_demo\inc" -I"C:\Users\kille\Desktop\Senior Project\SeniorProject\Hardware\lib_board_dp\inc" -I"C:\Users\kille\Desktop\Senior Project\SeniorProject\Hardware\lib_chip_8Nxx\inc" -I"C:\Users\kille\Desktop\Senior Project\SeniorProject\Hardware\app_demo\mods" -I"C:\Users\kille\Desktop\Senior Project\SeniorProject\Hardware\lib_board_dp\mods" -I"C:\Users\kille\Desktop\Senior Project\SeniorProject\Hardware\lib_chip_8Nxx\mods" -O0 -fno-common -g3 -Wall -Wextra -Wconversion -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


