################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../MSP_EXP432P401R_NoRTOS.cmd 

SYSCFG_SRCS += \
../servo.syscfg 

C_SRCS += \
../main_nortos.c \
../servo.c \
./syscfg/ti_drivers_config.c 

GEN_FILES += \
./syscfg/ti_drivers_config.c 

GEN_MISC_DIRS += \
./syscfg/ 

C_DEPS += \
./main_nortos.d \
./servo.d \
./syscfg/ti_drivers_config.d 

OBJS += \
./main_nortos.obj \
./servo.obj \
./syscfg/ti_drivers_config.obj 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/syscfg_c.rov.xs 

GEN_MISC_DIRS__QUOTED += \
"syscfg/" 

OBJS__QUOTED += \
"main_nortos.obj" \
"servo.obj" \
"syscfg/ti_drivers_config.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg/ti_drivers_config.h" \
"syscfg/syscfg_c.rov.xs" 

C_DEPS__QUOTED += \
"main_nortos.d" \
"servo.d" \
"syscfg/ti_drivers_config.d" 

GEN_FILES__QUOTED += \
"syscfg/ti_drivers_config.c" 

C_SRCS__QUOTED += \
"../main_nortos.c" \
"../servo.c" \
"./syscfg/ti_drivers_config.c" 

SYSCFG_SRCS__QUOTED += \
"../servo.syscfg" 


