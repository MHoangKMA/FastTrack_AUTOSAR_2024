################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/middleware/lpit_middleware/src/Mid_Lpit.c 

OBJS += \
./src/middleware/lpit_middleware/src/Mid_Lpit.o 

C_DEPS += \
./src/middleware/lpit_middleware/src/Mid_Lpit.d 


# Each subdirectory must supply rules for building sources it contributes
src/middleware/lpit_middleware/src/%.o: ../src/middleware/lpit_middleware/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/middleware/lpit_middleware/src/Mid_Lpit.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


