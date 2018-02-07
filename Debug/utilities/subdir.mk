################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_debug_console.c \
../utilities/fsl_misc_utilities.c \
../utilities/print_scan.c 

OBJS += \
./utilities/fsl_debug_console.o \
./utilities/fsl_misc_utilities.o \
./utilities/print_scan.o 

C_DEPS += \
./utilities/fsl_debug_console.d \
./utilities/fsl_misc_utilities.d \
./utilities/print_scan.d 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -DDEBUG -DCPU_MKL46Z256VLL4 -DFRDM_KL46Z -DFREEDOM -D"CPU_MKL46Z256VLH4" -I"../Sources" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"C:\Freescale\KSDK_1.3.0\platform\devices" -I"C:\Freescale\KSDK_1.3.0\platform\devices\MKL46Z4\include" -I"C:\Freescale\KSDK_1.3.0\platform\drivers\inc" -I"C:\Freescale\KSDK_1.3.0\platform\hal\inc" -I"C:\Freescale\KSDK_1.3.0\platform\osa\inc" -I"C:\Freescale\KSDK_1.3.0\platform\system\inc" -I"C:\Freescale\KSDK_1.3.0\platform\utilities\inc" -I"C:\Freescale\KSDK_1.3.0/platform/utilities/inc" -I"C:\Freescale\KSDK_1.3.0/platform/utilities/src" -I"C:\Users\rmeuth\workspace.kds\Template Project (Rename Me!)\utilities" -I"C:\Users\rmeuth\workspace.kds\Template Project (Rename Me!)\board" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


