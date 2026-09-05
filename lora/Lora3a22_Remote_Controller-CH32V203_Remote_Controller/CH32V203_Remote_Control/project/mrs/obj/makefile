################################################################################
# MRS Version: 2.5.0
# Automatically-generated file. Do not edit!
################################################################################
-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include wch_sdk/Startup/subdir.mk
-include wch_sdk/Peripheral/src/subdir.mk
-include wch_sdk/Core/subdir.mk
-include user/subdir.mk
-include software/subdir.mk
-include rtthread/src/subdir.mk
-include rtthread/libcpu/risc-v/common/subdir.mk
-include rtthread/components/finsh/subdir.mk
-include rtthread/subdir.mk
-include hardware/subdir.mk
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
CH32V203_Remote_Control.hex \

SECONDARY_LIST += \
CH32V203_Remote_Control.lst \

SECONDARY_SIZE += \
CH32V203_Remote_Control.siz \


# All Target
all: 
	$(MAKE) --no-print-directory main-build 

main-build: CH32V203_Remote_Control.elf secondary-outputs

# Tool invocations
CH32V203_Remote_Control.elf: $(OBJS) $(USER_OBJS_ESCAPE)
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -T "d:\Users\CH32V203\lora\Lora3a22_Remote_Controller-CH32V203_Remote_Controller\CH32V203_Remote_Control\wch_sdk\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"CH32V203_Remote_Control.map" --specs=nano.specs --specs=nosys.specs -Wl,--print-memory-usage -o "CH32V203_Remote_Control.elf" $(OBJS) $(LIBS)
CH32V203_Remote_Control.hex: CH32V203_Remote_Control.elf
	@	riscv-none-embed-objcopy -O ihex "CH32V203_Remote_Control.elf" "CH32V203_Remote_Control.hex"
CH32V203_Remote_Control.lst: CH32V203_Remote_Control.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw "CH32V203_Remote_Control.elf" > "CH32V203_Remote_Control.lst"
CH32V203_Remote_Control.siz: CH32V203_Remote_Control.elf
	riscv-none-embed-size --format=berkeley "CH32V203_Remote_Control.elf"

# Other Targets
clean:
	-$(RM) $(DIR_OBJS) $(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE) CH32V203_Remote_Control.elf
	-$(RM) $(DIR_EXPANDS) $(CALLGRAPH_DOT)
	-$(RM) $(DIR_DEPS)
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets