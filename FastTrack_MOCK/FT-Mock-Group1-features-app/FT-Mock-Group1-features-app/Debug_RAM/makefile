################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/middleware/lpit_middleware/src/subdir.mk
-include src/middleware/can_middleware/src/subdir.mk
-include src/driver/uart_driver/src/subdir.mk
-include src/driver/port_driver/src/subdir.mk
-include src/driver/lpit_driver/src/subdir.mk
-include src/driver/gpio_driver/src/subdir.mk
-include src/driver/clock_driver/src/subdir.mk
-include src/driver/can_driver/src/subdir.mk
-include src/driver/adc_driver/src/subdir.mk
-include src/driver/NVIC/src/subdir.mk
-include src/app/node_speed/src/subdir.mk
-include src/app/node_forwarder/src/subdir.mk
-include src/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_SIZE += \
project_folder_structure.siz \


# All Target
all: project_folder_structure.elf secondary-outputs

# Tool invocations
project_folder_structure.elf: $(OBJS) G:/APS_Trainning/Mock_Prj_Gr1/FT-Mock-Group1-1/Project_Settings/Linker_Files/S32K144_64_ram.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Standard S32DS C Linker'
	arm-none-eabi-gcc -o "project_folder_structure.elf" "@project_folder_structure.args"  $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

project_folder_structure.siz: project_folder_structure.elf
	@echo 'Invoking: Standard S32DS Print Size'
	arm-none-eabi-size --format=berkeley project_folder_structure.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ./*
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
