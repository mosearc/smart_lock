################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/home/mose/ti/ccs1250/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/mose/Desktop/Smart_Lock/simplelink_msp432p4_sdk_3_40_01_02/source" --include_path="/home/mose/Desktop/vaffanculoo/smart_lock3" --include_path="/home/mose/Desktop/vaffanculoo/smart_lock3/Debug" --include_path="/home/mose/Desktop/courses/Embedded/simplelink_msp432p4_sdk_3_40_01_02/source" --include_path="/home/mose/Desktop/courses/Embedded/simplelink_msp432p4_sdk_3_40_01_02/source/third_party/CMSIS/Include" --include_path="/home/mose/Desktop/courses/Embedded/simplelink_msp432p4_sdk_3_40_01_02/kernel/nortos" --include_path="/home/mose/Desktop/courses/Embedded/simplelink_msp432p4_sdk_3_40_01_02/kernel/nortos/posix" --include_path="/home/mose/ti/ccs1250/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --advice:power=none -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="/home/mose/Desktop/vaffanculoo/smart_lock3/Debug/syscfg" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1524532465: ../servo.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"/home/mose/ti/ccs1250/ccs/utils/sysconfig_1.18.0/sysconfig_cli.sh" -s "/home/mose/Desktop/courses/Embedded/simplelink_msp432p4_sdk_3_40_01_02/.metadata/product.json" --script "/home/mose/Desktop/vaffanculoo/smart_lock3/servo.syscfg" -o "syscfg" --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_drivers_config.c: build-1524532465 ../servo.syscfg
syscfg/ti_drivers_config.h: build-1524532465
syscfg/syscfg_c.rov.xs: build-1524532465
syscfg/: build-1524532465

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/home/mose/ti/ccs1250/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/mose/Desktop/Smart_Lock/simplelink_msp432p4_sdk_3_40_01_02/source" --include_path="/home/mose/Desktop/vaffanculoo/smart_lock3" --include_path="/home/mose/Desktop/vaffanculoo/smart_lock3/Debug" --include_path="/home/mose/Desktop/courses/Embedded/simplelink_msp432p4_sdk_3_40_01_02/source" --include_path="/home/mose/Desktop/courses/Embedded/simplelink_msp432p4_sdk_3_40_01_02/source/third_party/CMSIS/Include" --include_path="/home/mose/Desktop/courses/Embedded/simplelink_msp432p4_sdk_3_40_01_02/kernel/nortos" --include_path="/home/mose/Desktop/courses/Embedded/simplelink_msp432p4_sdk_3_40_01_02/kernel/nortos/posix" --include_path="/home/mose/ti/ccs1250/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --advice:power=none -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="/home/mose/Desktop/vaffanculoo/smart_lock3/Debug/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


