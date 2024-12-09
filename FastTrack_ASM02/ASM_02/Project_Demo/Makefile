###############################################################################
# Struct folder directories
PROJECT_NAME        := S32K144_Demo
PROJECT_DIR         := .
# TODO: Define all macro for directories use in makefile
PROJECT_SETTING_DIR := $(PROJECT_DIR)/Project_Settings
SRC_DIR             := $(PROJECT_DIR)/src
INC_DIR             := $(PROJECT_DIR)/include
OUTPUT_DIR          := $(PROJECT_DIR)/output
OBJECT_DIR          := $(OUTPUT_DIR)/object
DEPEND_DIR          := $(OUTPUT_DIR)/depend
NEWLIB              := $(GCC_DIR)/arm-none-eabi/lib
# Create output folder directory if it hasn't exist
$(OUTPUT_DIR):
	@mkdir $(OUTPUT_DIR)
	@mkdir $(OBJECT_DIR)
	@mkdir $(DEPEND_DIR)
##############################################################################
#List of directories for all source files
SRC_DIRS  += $(shell find $(SRC_DIR) -type f -name "*.c" -exec dirname {} \; | uniq) \
			 $(shell find $(PROJECT_SETTING_DIR) -type f -name "*.c" -exec dirname {} \; | uniq)

# List all source file
SRC_FILES := $(foreach SRC_DIR, $(SRC_DIRS),$(notdir $(wildcard $(SRC_DIR)/*.c)))
ASM_SRC_FILES := startup_S32K144.S

# List of directories for all header files
INC_DIRS  += $(shell find $(SRC_DIR) -type f -name "*.h" -exec dirname {} \; | uniq) \
			 $(INC_DIR)

# Option -I for pass Include directories (List all include path)
INC_PATHS := $(addprefix -I, $(INC_DIRS))

# Elf name and .Map name
ELF_FILE := S32K144_Demo.elf
MAP_FILE := S32K144_Demo.map

SEPARATOR=======================================================================
###############################################################################
# Path to look up .c file, .h file , .s file and .o file
vpath %.c $(addsuffix :, $(SRC_DIRS))
vpath %.h $(addsuffix :, $(INC_DIRS))
vpath %.S $(PROJECT_SETTING_DIR)/Startup_Code
vpath %.o $(OBJECT_DIR)

# List all objects file for linking
OBJECT_FILES += $(SRC_FILES:.c=.o) \
			 	$(ASM_SRC_FILES:.S=.o)

###############################################################################
# Set dependency option, write to a temporary file
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPEND_DIR)/$*.d

# List of all the dependency files that could exist
DEPEND_FILES := $(patsubst %,$(DEPEND_DIR)/%,$(notdir $(SRC_FILES:%.c=%.d)))

# Mention each dependency file as a target, so that make won't fail
# If the file doesn't exist
$(DEPEND_FILES):

# Include dependency files that exist. use wildcard to filter
# Non-existent files.
include $(wildcard $(DEPEND_FILES))

###############################################################################
# Macro for Compiler, assembler, linker tool use $(GCC_PATH)
CC := $(GCC_DIR)/bin/arm-none-eabi-gcc.exe
LD := $(GCC_DIR)/bin/arm-none-eabi-gcc.exe
AS := $(GCC_DIR)/bin/arm-none-eabi-gcc.exe

# Set compiler option
CCOPT 	:= $(INC_PATHS) \
		-Os \
		-g3 \
		-Wall \
		-c \
		-fmessage-length=0 \
		-ffunction-sections \
		-fdata-sections \
		-mcpu=cortex-m4 \
		-mthumb \
		-mlittle-endian \
		-specs=nano.specs \
		-specs=nosys.specs \
        --sysroot="$(NEWLIB)" \
		-DCPU_S32K144HFT0VLLT

# Select linker file depend on write to FLASH or RAM
ifeq ($(LOAD_TO),FLASH)
	AS_START_FROM := -DSTART_FROM_FLASH
	LINKER_FILE := $(PROJECT_SETTING_DIR)/Linker_Files/S32K144_64_flash.ld
else
	AS_START_FROM := -DSTART_FROM_RAM
	LINKER_FILE := $(PROJECT_SETTING_DIR)/Linker_Files/S32K144_64_ram.ld
endif

# Set linker option
LDOPT := -Wl,-Map,"$(OUTPUT_DIR)/$(MAP_FILE)" \
		 -Xlinker \
		 --gc-sections \
		 -n \
		 -mcpu=cortex-m4 \
		 -mthumb \
		 -mlittle-endian \
         -T $(LINKER_FILE) \
		 -specs=nosys.specs \
         --sysroot="$(NEWLIB)"

# Set Assembly option
ASOPT := -c \
         $(INC_PATHS) \
		 $(AS_START_FROM) \
		 -g3 \
		 -mcpu=cortex-m4 \
		 -mthumb \
		 -mlittle-endian \
		 -specs=nano.specs \
		 -specs=nosys.specs \
         --sysroot="$(NEWLIB)"

###############################################################################
# Compile target, dependency: .c source file
# Auto dependency generation
.PHONY: compile
compile: $(OBJECT_FILES)
#	@echo "==> COMPILE COMPLETED!."

# Target used to compile C source files
%.o: %.c 
	@echo $(SEPARATOR)
	@echo -e "\e[1;95mCOMPILING C FILE:\e[0m"
	@echo -e "\e[1;33mCompiling $(OBJECT_DIR)/$@\e[0m"
	@echo -e "\e[1;32mCOMPILING Completed! : $<\e[0m"
	@$(CC) $(CCOPT) $(DEPFLAGS) $< -o $(OBJECT_DIR)/$@

# Target used to compile Assembler files
%.o: %.S 
	@echo $(SEPARATOR)
	@echo -e "\e[1;95mCOMPILING ASSEMBLER FILE:\e[0m"
	@echo -e "\e[1;33mCompiling $(OBJECT_DIR)/$@\e[0m"
	@echo -e "\e[1;32mCOMPILING Completed! : $<\e[0m"
	@$(AS) $(ASOPT) $(DEPFLAGS) $< -o $(OBJECT_DIR)/$@
###############################################################################
# Make build: Build target dependency object files
.PHONY: build
build: $(OUTPUT_DIR) compile $(ELF_FILE)
	@echo -e "==> \e[1;32mBUILD on $(LOAD_TO) Complete!\e[0m"
	@echo -e "==> \e[1;32m.ELF file: $(ELF_FILE)\e[0m"
	@echo -e "==> \e[1;32m.MAP file: $(ELF_FILE)\e[0m"

# Target used to link compiled sources
$(ELF_FILE): 
	@echo $(SEPARATOR)
	@echo -e "\e[1;95mBuilding target: $@\e[0m"
	@echo -e "\e[1;95mInvoking: Standard S32DS C Linker\e[0m"
	@echo -e "\e[1;33mLinking to $(OUTPUT_DIR)/$@\e[0m"
	@$(LD) -o $(OUTPUT_DIR)/$@ $(addprefix $(OBJECT_DIR)/,$(OBJECT_FILES)) $(LDOPT)

###############################################################################
# Make clean: Clean all file that generated by building process.
.PHONY: clean
clean:
	@echo $(SEPARATOR)
	@if [ -d $(OBJECT_DIR) ]; then \
		rm -rf $(OUTPUT_DIR); \
		echo -e "==> \e[1;32mCleaning... Completed!\e[0m"; \
	else \
		echo -e "==> \e[1;31mCleaning Fail! \e[0m"; \
	fi
###############################################################################
# Target to print variables
print-%:
	@echo $($*)