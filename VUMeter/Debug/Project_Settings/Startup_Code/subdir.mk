################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Project_Settings/Startup_Code/startup.c \
../Project_Settings/Startup_Code/system_MK64F12.c 

S_UPPER_SRCS += \
../Project_Settings/Startup_Code/startup_MK64F12.S 

OBJS += \
./Project_Settings/Startup_Code/startup.o \
./Project_Settings/Startup_Code/startup_MK64F12.o \
./Project_Settings/Startup_Code/system_MK64F12.o 

C_DEPS += \
./Project_Settings/Startup_Code/startup.d \
./Project_Settings/Startup_Code/system_MK64F12.d 

S_UPPER_DEPS += \
./Project_Settings/Startup_Code/startup_MK64F12.d 


# Each subdirectory must supply rules for building sources it contributes
Project_Settings/Startup_Code/%.o: ../Project_Settings/Startup_Code/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK64FN1M0VMD12" -D"CPU_MK64FN1M0VLL12" -D"ARM_MATH_CM4" -D"_FPU_PRESENT" -I"../Sources" -I"C:\Freescale\KSDK_1.3.0/platform/osa/inc" -I"C:\Freescale\KSDK_1.3.0/platform/system/inc" -I"C:\Freescale\KSDK_1.3.0/platform/drivers/src/edma" -I"C:\Freescale\KSDK_1.3.0/platform/drivers/src/adc16" -I"C:\Freescale\KSDK_1.3.0/platform/utilities/src" -I"C:\Freescale\KSDK_1.3.0/platform/utilities/inc" -I"C:\Freescale\KSDK_1.3.0/platform/drivers/inc" -I"C:\Freescale\KSDK_1.3.0/platform/hal/inc" -I"C:\Freescale\KSDK_1.3.0/platform/drivers/src/adc16" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MK64F12/include" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Project_Settings/Startup_Code/%.o: ../Project_Settings/Startup_Code/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -x assembler-with-cpp -D"NO_INIT_DATA_BSS" -D"_STACK_SIZE=0x0400" -D"_HEAP_SIZE=0x00" -D"_START=main" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


