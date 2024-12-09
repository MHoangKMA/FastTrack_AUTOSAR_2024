################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/driver/lpit_driver/src/Drv_Lpit.c 

OBJS += \
./src/driver/lpit_driver/src/Drv_Lpit.o 

C_DEPS += \
./src/driver/lpit_driver/src/Drv_Lpit.d 


# Each subdirectory must supply rules for building sources it contributes
src/driver/lpit_driver/src/%.o: ../src/driver/lpit_driver/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/driver/lpit_driver/src/Drv_Lpit.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


