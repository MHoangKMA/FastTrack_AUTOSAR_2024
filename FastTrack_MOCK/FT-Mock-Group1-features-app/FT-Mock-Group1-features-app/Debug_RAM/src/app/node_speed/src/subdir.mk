################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/app/node_speed/src/node_speed.c 

OBJS += \
./src/app/node_speed/src/node_speed.o 

C_DEPS += \
./src/app/node_speed/src/node_speed.d 


# Each subdirectory must supply rules for building sources it contributes
src/app/node_speed/src/%.o: ../src/app/node_speed/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/app/node_speed/src/node_speed.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


