################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/middleware/can_middleware/src/MIDDLE_FlexCAN.c 

OBJS += \
./src/middleware/can_middleware/src/MIDDLE_FlexCAN.o 

C_DEPS += \
./src/middleware/can_middleware/src/MIDDLE_FlexCAN.d 


# Each subdirectory must supply rules for building sources it contributes
src/middleware/can_middleware/src/%.o: ../src/middleware/can_middleware/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/middleware/can_middleware/src/MIDDLE_FlexCAN.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


