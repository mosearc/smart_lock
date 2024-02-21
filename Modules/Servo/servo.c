#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <NoRTOS.h>

/* Driver configuration */
#include <ti/drivers/Board.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>
#include "ti_drivers_config.h"

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

#include "servo.h"

uint32_t pwmPeriodServo = 20000; //20ms
uint32_t dutyCycleServo = 0;
PWM_Handle pwmHandleServo;

void configServo(void *arg0)
{
    PWM_init();
    PWM_Params pwmParams;

    PWM_Params_init(&pwmParams);
    pwmParams.dutyUnits = PWM_DUTY_US;
    pwmParams.dutyValue = 0;
    pwmParams.periodUnits = PWM_PERIOD_US;
    pwmParams.periodValue = pwmPeriodServo;
    pwmHandleServo = PWM_open(CONFIG_PWM_0, &pwmParams);
    if(pwmHandleServo == NULL){
        while (1);
    }

    PWM_start(pwmHandleServo);
    PWM_setDuty(pwmHandleServo, dutyCycleServo+1500); //90 gradi
}

void openDoor(void *arg0){
    PWM_setDuty(pwmHandleServo, dutyCycleServo+1500); //90 gradi
}

void closeDoor(void *arg0){
    PWM_setDuty(pwmHandleServo, dutyCycleServo+2000); //180 gradi
}

