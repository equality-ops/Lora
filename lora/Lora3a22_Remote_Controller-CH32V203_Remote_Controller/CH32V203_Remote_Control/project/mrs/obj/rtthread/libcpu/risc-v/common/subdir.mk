################################################################################
# MRS Version: 2.5.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/libcpu/risc-v/common/cpuport.c 

C_DEPS += \
./rtthread/libcpu/risc-v/common/cpuport.d 

S_UPPER_SRCS += \
d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/libcpu/risc-v/common/context_gcc.S \
d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/libcpu/risc-v/common/interrupt_gcc.S 

S_UPPER_DEPS += \
./rtthread/libcpu/risc-v/common/context_gcc.d \
./rtthread/libcpu/risc-v/common/interrupt_gcc.d 

OBJS += \
./rtthread/libcpu/risc-v/common/context_gcc.o \
./rtthread/libcpu/risc-v/common/cpuport.o \
./rtthread/libcpu/risc-v/common/interrupt_gcc.o 

DIR_OBJS += \
./rtthread/libcpu/risc-v/common/*.o \

DIR_DEPS += \
./rtthread/libcpu/risc-v/common/*.d \

DIR_EXPANDS += \
./rtthread/libcpu/risc-v/common/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
rtthread/libcpu/risc-v/common/cpuport.o: d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/libcpu/risc-v/common/cpuport.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/config" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/hardware" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/software" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/components" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/components/finsh" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/include" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/include/libc" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/libcpu" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/libcpu/risc-v" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/libcpu/risc-v/common" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/src" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/project/user" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/wch_sdk" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/wch_sdk/Core" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/wch_sdk/Ld" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/wch_sdk/Peripheral" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/wch_sdk/Peripheral/inc" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/wch_sdk/Peripheral/src" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/wch_sdk/Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

rtthread/libcpu/risc-v/common/context_gcc.o: d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/libcpu/risc-v/common/context_gcc.S
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -x assembler-with-cpp -DCH32V20x_D8W -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/components" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/wch_sdk/Startup" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/include" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/include/libc" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/libcpu" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/libcpu/risc-v" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/libcpu/risc-v/common" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
rtthread/libcpu/risc-v/common/interrupt_gcc.o: d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/libcpu/risc-v/common/interrupt_gcc.S
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -x assembler-with-cpp -DCH32V20x_D8W -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/components" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/wch_sdk/Startup" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/include" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/include/libc" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/libcpu" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/libcpu/risc-v" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/libcpu/risc-v/common" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

