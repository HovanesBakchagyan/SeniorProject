################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bussync_8Nxx.c \
../src/clock_8Nxx.c \
../src/eeprom_8Nxx.c \
../src/flash_8Nxx.c \
../src/gpio_8Nxx.c \
../src/i2c_8Nxx.c \
../src/iap_8Nxx.c \
../src/iocon_8Nxx.c \
../src/nfc_8Nxx.c \
../src/pmu_8Nxx.c \
../src/rtc_8Nxx.c \
../src/ssp_8Nxx.c \
../src/syscon_8Nxx.c \
../src/timer_8Nxx.c \
../src/tsen_8Nxx.c \
../src/wwdt_8Nxx.c 

OBJS += \
./src/bussync_8Nxx.o \
./src/clock_8Nxx.o \
./src/eeprom_8Nxx.o \
./src/flash_8Nxx.o \
./src/gpio_8Nxx.o \
./src/i2c_8Nxx.o \
./src/iap_8Nxx.o \
./src/iocon_8Nxx.o \
./src/nfc_8Nxx.o \
./src/pmu_8Nxx.o \
./src/rtc_8Nxx.o \
./src/ssp_8Nxx.o \
./src/syscon_8Nxx.o \
./src/timer_8Nxx.o \
./src/tsen_8Nxx.o \
./src/wwdt_8Nxx.o 

C_DEPS += \
./src/bussync_8Nxx.d \
./src/clock_8Nxx.d \
./src/eeprom_8Nxx.d \
./src/flash_8Nxx.d \
./src/gpio_8Nxx.d \
./src/i2c_8Nxx.d \
./src/iap_8Nxx.d \
./src/iocon_8Nxx.d \
./src/nfc_8Nxx.d \
./src/pmu_8Nxx.d \
./src/rtc_8Nxx.d \
./src/ssp_8Nxx.d \
./src/syscon_8Nxx.d \
./src/timer_8Nxx.d \
./src/tsen_8Nxx.d \
./src/wwdt_8Nxx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=c99 -D__REDLIB__ -DDEBUG -D__CODE_RED -DCORE_M0PLUS -I"C:\Users\kille\Desktop\Senior Project\SeniorProject\Hardware\lib_chip_8Nxx\inc" -I"C:\Users\kille\Desktop\Senior Project\SeniorProject\Hardware\lib_chip_8Nxx\mods" -O0 -g3 -pedantic -Wall -Wextra -Wconversion -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


