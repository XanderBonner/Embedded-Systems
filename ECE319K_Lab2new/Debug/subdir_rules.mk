################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Clock.o: C:/Users/jossx/OneDrive/Desktop/319K\ Labs/MSPM0_ValvanoWare/inc/Clock.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new" -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new/Debug" -I"C:/ti/mspm0_sdk_1_10_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_10_00_05/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"Clock.d_raw" -MT"Clock.o" -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new/Debug/syscfg" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Dump.o: C:/Users/jossx/OneDrive/Desktop/319K\ Labs/MSPM0_ValvanoWare/inc/Dump.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new" -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new/Debug" -I"C:/ti/mspm0_sdk_1_10_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_10_00_05/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"Dump.d_raw" -MT"Dump.o" -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new/Debug/syscfg" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.o: ../%.s $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new" -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new/Debug" -I"C:/ti/mspm0_sdk_1_10_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_10_00_05/source" -D__MSPM0G3507__ -gdwarf-3 -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new/Debug/syscfg" $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new" -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new/Debug" -I"C:/ti/mspm0_sdk_1_10_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_10_00_05/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new/Debug/syscfg" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

LaunchPad.o: C:/Users/jossx/OneDrive/Desktop/319K\ Labs/MSPM0_ValvanoWare/inc/LaunchPad.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new" -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new/Debug" -I"C:/ti/mspm0_sdk_1_10_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_10_00_05/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"LaunchPad.d_raw" -MT"LaunchPad.o" -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new/Debug/syscfg" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Timer.o: C:/Users/jossx/OneDrive/Desktop/319K\ Labs/MSPM0_ValvanoWare/inc/Timer.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new" -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new/Debug" -I"C:/ti/mspm0_sdk_1_10_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_10_00_05/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"Timer.d_raw" -MT"Timer.o" -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new/Debug/syscfg" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

UART.o: C:/Users/jossx/OneDrive/Desktop/319K\ Labs/MSPM0_ValvanoWare/inc/UART.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new" -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new/Debug" -I"C:/ti/mspm0_sdk_1_10_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_10_00_05/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"UART.d_raw" -MT"UART.o" -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new/Debug/syscfg" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

UARTbusywait.o: C:/Users/jossx/OneDrive/Desktop/319K\ Labs/MSPM0_ValvanoWare/inc/UARTbusywait.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new" -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new/Debug" -I"C:/ti/mspm0_sdk_1_10_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_10_00_05/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"UARTbusywait.d_raw" -MT"UARTbusywait.o" -I"C:/Users/jossx/OneDrive/Desktop/319K Labs/MSPM0_ValvanoWare/ECE319K_Lab2new/Debug/syscfg" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


