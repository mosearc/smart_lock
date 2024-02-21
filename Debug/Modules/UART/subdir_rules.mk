################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Modules/UART/%.obj: ../Modules/UART/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/home/mose/ti/ccs1250/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/mose/Desktop/Smart_Lock/simplelink_msp432p4_sdk_3_40_01_02/source" --include_path="/home/mose/Desktop/vaffanculoo/smart_lock3" --include_path="/home/mose/Desktop/vaffanculoo/smart_lock3/Debug" --include_path="/home/mose/Desktop/courses/Embedded/simplelink_msp432p4_sdk_3_40_01_02/source" --include_path="/home/mose/Desktop/courses/Embedded/simplelink_msp432p4_sdk_3_40_01_02/source/third_party/CMSIS/Include" --include_path="/home/mose/Desktop/courses/Embedded/simplelink_msp432p4_sdk_3_40_01_02/kernel/nortos" --include_path="/home/mose/Desktop/courses/Embedded/simplelink_msp432p4_sdk_3_40_01_02/kernel/nortos/posix" --include_path="/home/mose/ti/ccs1250/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --advice:power=none -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="Modules/UART/$(basename $(<F)).d_raw" --include_path="/home/mose/Desktop/vaffanculoo/smart_lock3/Debug/syscfg" --obj_directory="Modules/UART" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


