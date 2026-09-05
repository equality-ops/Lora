################################################################################
# MRS Version: 2.5.0
# Automatically-generated file. Do not edit!
################################################################################
-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include Peripheral/src/subdir.mk
-include Debug/subdir.mk
-include Core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
LORA_control.hex \

SECONDARY_LIST += \
LORA_control.lst \

SECONDARY_SIZE += \
LORA_control.siz \


# All Target
all: 
	$(MAKE) --no-print-directory main-build 

main-build: LORA_control.elf secondary-outputs

# Tool invocations
LORA_control.elf: $(OBJS) $(USER_OBJS_ESCAPE)
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -T "d:\Users\CH32V203\lora\LORA_control\LORA_control\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"LORA_control.map" --specs=nano.specs --specs=nosys.specs -o "LORA_control.elf" $(OBJS) $(LIBS)
LORA_control.hex: LORA_control.elf
	@	riscv-none-embed-objcopy -O ihex "LORA_control.elf" "LORA_control.hex"
LORA_control.lst: LORA_control.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw "LORA_control.elf" > "LORA_control.lst"
LORA_control.siz: LORA_control.elf
	riscv-none-embed-size --format=berkeley "LORA_control.elf"

# Other Targets
clean:
	-$(RM) $(DIR_OBJS) $(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE) LORA_control.elf
	-$(RM) $(DIR_EXPANDS) $(CALLGRAPH_DOT)
	-$(RM) $(DIR_DEPS)
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets