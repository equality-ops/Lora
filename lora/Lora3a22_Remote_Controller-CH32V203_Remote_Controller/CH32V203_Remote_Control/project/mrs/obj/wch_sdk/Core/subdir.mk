################################################################################
# MRS Version: 2.5.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/wch_sdk/Core/core_riscv.c 

C_DEPS += \
./wch_sdk/Core/core_riscv.d 

OBJS += \
./wch_sdk/Core/core_riscv.o 

DIR_OBJS += \
./wch_sdk/Core/*.o \

DIR_DEPS += \
./wch_sdk/Core/*.d \

DIR_EXPANDS += \
./wch_sdk/Core/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
wch_sdk/Core/core_riscv.o: d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/wch_sdk/Core/core_riscv.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/config" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/hardware" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/software" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/components" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/components/finsh" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/include" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/include/libc" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/libcpu" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/libcpu/risc-v" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/libcpu/risc-v/common" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/src" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/project/user" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/wch_sdk" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/wch_sdk/Core" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/wch_sdk/Ld" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/wch_sdk/Peripheral" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/wch_sdk/Peripheral/inc" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/wch_sdk/Peripheral/src" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/wch_sdk/Startup" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

