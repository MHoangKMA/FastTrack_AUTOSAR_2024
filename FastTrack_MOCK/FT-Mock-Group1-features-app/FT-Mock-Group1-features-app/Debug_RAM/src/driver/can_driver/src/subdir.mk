################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/driver/can_driver/src/FlexCAN.c 

OBJS += \
./src/driver/can_driver/src/FlexCAN.o 

C_DEPS += \
./src/driver/can_driver/src/FlexCAN.d 


# Each subdirectory must supply rules for building sources it contributes
src/driver/can_driver/src/%.o: ../src/driver/can_driver/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/driver/can_driver/src/FlexCAN.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


