################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/crp.c \
../src/main.c \
../src/timer.c 

OBJS += \
./src/crp.o \
./src/main.o \
./src/timer.o 

C_DEPS += \
./src/crp.d \
./src/main.d \
./src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=c99 -DNDEBUG -D__CODE_RED -DCORE_M0PLUS -D__REDLIB__ -I"C:\Users\nxp73930\Documents\MCUXpressoIDE_10.1.0_569_prc2g\workspace\app_demo\inc" -I"C:\Users\nxp73930\Documents\MCUXpressoIDE_10.1.0_569_prc2g\workspace\lib_board_dp\inc" -I"C:\Users\nxp73930\Documents\MCUXpressoIDE_10.1.0_569_prc2g\workspace\lib_chip_8Nxx\inc" -I"C:\Users\nxp73930\Documents\MCUXpressoIDE_10.1.0_569_prc2g\workspace\app_demo\mods" -I"C:\Users\nxp73930\Documents\MCUXpressoIDE_10.1.0_569_prc2g\workspace\lib_board_dp\mods" -I"C:\Users\nxp73930\Documents\MCUXpressoIDE_10.1.0_569_prc2g\workspace\lib_chip_8Nxx\mods" -Os -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


