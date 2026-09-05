################################################################################
# MRS Version: 2.5.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/wch_sdk/Startup/startup_ch32v20x_D6.S 

S_UPPER_DEPS += \
./wch_sdk/Startup/startup_ch32v20x_D6.d 

OBJS += \
./wch_sdk/Startup/startup_ch32v20x_D6.o 

DIR_OBJS += \
./wch_sdk/Startup/*.o \

DIR_DEPS += \
./wch_sdk/Startup/*.d \

DIR_EXPANDS += \
./wch_sdk/Startup/*.234r.expand \


# Each subdirectory must supply rules for building sources it contributes
wch_sdk/Startup/startup_ch32v20x_D6.o: d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/wch_sdk/Startup/startup_ch32v20x_D6.S
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -x assembler-with-cpp -DCH32V20x_D8W -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/components" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/wch_sdk/Startup" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/include" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/include/libc" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/libcpu" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/libcpu/risc-v" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/libcpu/risc-v/common" -I"d:/Users/CH32V203/lora/Lora3a22_Remote_Controller-CH32V203_Remote_Controller/CH32V203_Remote_Control/rtthread/src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

