################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/app/node_forwarder/src/node_forwarder.c 

OBJS += \
./src/app/node_forwarder/src/node_forwarder.o 

C_DEPS += \
./src/app/node_forwarder/src/node_forwarder.d 


# Each subdirectory must supply rules for building sources it contributes
src/app/node_forwarder/src/%.o: ../src/app/node_forwarder/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/app/node_forwarder/src/node_forwarder.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


