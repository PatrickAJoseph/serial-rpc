################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'GNU Compiler - building file: "$<"'
	"C:/ti/gcc-arm-none-eabi_9_3_1/bin/arm-none-eabi-gcc-9.3.1.exe" -c -mcpu=cortex-m0plus -march=armv6-m -mthumb -mfloat-abi=soft -D_REENT_SMALL -I"F:/content_writing/serial-rpc/serial-rpc-example/serial-rpc-example" -I"F:/content_writing/serial-rpc/serial-rpc-example/serial-rpc-example/Debug" -I"C:/ti/simplelink_lowpower_f3_sdk_9_14_01_16/source" -I"C:/ti/simplelink_lowpower_f3_sdk_9_14_01_16/kernel/freertos" -I"C:/ti/simplelink_lowpower_f3_sdk_9_14_01_16/source/ti/posix/gcc" -I"C:/ti/simplelink_lowpower_f3_sdk_9_14_01_16/source/third_party/freertos/include" -I"C:/ti/simplelink_lowpower_f3_sdk_9_14_01_16/source/third_party/freertos/portable/GCC/ARM_CM0" -I"C:/ti/gcc-arm-none-eabi_9_3_1/arm-none-eabi/include/newlib-nano" -I"C:/ti/gcc-arm-none-eabi_9_3_1/arm-none-eabi/include" -O3 -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"F:/content_writing/serial-rpc/serial-rpc-example/serial-rpc-example/Debug/syscfg" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1681740978: ../serial-rpc-example.syscfg
	@echo 'SysConfig - building file: "$<"'
	"C:/ti/sysconfig_1.23.2/sysconfig_cli.bat" -s "C:/ti/simplelink_lowpower_f3_sdk_9_14_01_16/.metadata/product.json" --script "F:/content_writing/serial-rpc/serial-rpc-example/serial-rpc-example/serial-rpc-example.syscfg" -o "syscfg" --compiler gcc
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_devices_config.c: build-1681740978 ../serial-rpc-example.syscfg
syscfg/ti_drivers_config.c: build-1681740978
syscfg/ti_drivers_config.h: build-1681740978
syscfg/ti_utils_build_linker.cmd.genlibs: build-1681740978
syscfg/ti_utils_build_linker.cmd.genmap: build-1681740978
syscfg/ti_utils_build_compiler.opt: build-1681740978
syscfg/syscfg_c.rov.xs: build-1681740978
syscfg/FreeRTOSConfig.h: build-1681740978
syscfg/ti_freertos_config.c: build-1681740978
syscfg/ti_freertos_portable_config.c: build-1681740978
syscfg: build-1681740978

syscfg/%.o: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'GNU Compiler - building file: "$<"'
	"C:/ti/gcc-arm-none-eabi_9_3_1/bin/arm-none-eabi-gcc-9.3.1.exe" -c -mcpu=cortex-m0plus -march=armv6-m -mthumb -mfloat-abi=soft -D_REENT_SMALL -I"F:/content_writing/serial-rpc/serial-rpc-example/serial-rpc-example" -I"F:/content_writing/serial-rpc/serial-rpc-example/serial-rpc-example/Debug" -I"C:/ti/simplelink_lowpower_f3_sdk_9_14_01_16/source" -I"C:/ti/simplelink_lowpower_f3_sdk_9_14_01_16/kernel/freertos" -I"C:/ti/simplelink_lowpower_f3_sdk_9_14_01_16/source/ti/posix/gcc" -I"C:/ti/simplelink_lowpower_f3_sdk_9_14_01_16/source/third_party/freertos/include" -I"C:/ti/simplelink_lowpower_f3_sdk_9_14_01_16/source/third_party/freertos/portable/GCC/ARM_CM0" -I"C:/ti/gcc-arm-none-eabi_9_3_1/arm-none-eabi/include/newlib-nano" -I"C:/ti/gcc-arm-none-eabi_9_3_1/arm-none-eabi/include" -O3 -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"syscfg/$(basename $(<F)).d_raw" -MT"$(@)" -I"F:/content_writing/serial-rpc/serial-rpc-example/serial-rpc-example/Debug/syscfg" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


