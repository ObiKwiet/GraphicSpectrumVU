################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/edma_init.c \
../Sources/main.c \
../Sources/perf_init.c 

OBJS += \
./Sources/edma_init.o \
./Sources/main.o \
./Sources/perf_init.o 

C_DEPS += \
./Sources/edma_init.d \
./Sources/main.d \
./Sources/perf_init.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK64FN1M0VMD12" -D"CPU_MK64FN1M0VLL12" -D"ARM_MATH_CM4" -D"_FPU_PRESENT" -I"../Sources" -I"C:\Freescale\KSDK_1.3.0/platform/osa/inc" -I"C:\Freescale\KSDK_1.3.0/platform/system/inc" -I"C:\Freescale\KSDK_1.3.0/platform/drivers/src/edma" -I"C:\Freescale\KSDK_1.3.0/platform/drivers/src/adc16" -I"C:\Freescale\KSDK_1.3.0/platform/utilities/src" -I"C:\Freescale\KSDK_1.3.0/platform/utilities/inc" -I"C:\Freescale\KSDK_1.3.0/platform/drivers/inc" -I"C:\Freescale\KSDK_1.3.0/platform/hal/inc" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MK64F12/include" -I"C:\Downloads\Projects\GraphicSpectrumVU\VUMeter\board" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


