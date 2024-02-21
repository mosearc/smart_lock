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

#include "buzzer.h"

uint32_t pwmPeriodBuzzer = 3000; //20ms
uint32_t dutyCycleBuzzer = 0;
PWM_Handle pwmHandleBuzzer;

void configBuzzer(void *arg0)
{
    PWM_Params pwmParams;

    PWM_init();

    PWM_Params_init(&pwmParams);
    pwmParams.dutyUnits = PWM_DUTY_US;
    pwmParams.dutyValue = 0;
    pwmParams.periodUnits = PWM_PERIOD_US;
    pwmParams.periodValue = pwmPeriodBuzzer;

    pwmHandleBuzzer = PWM_open(CONFIG_PWM_1, &pwmParams);
    if (pwmHandleBuzzer == NULL) {
        while (1){
            printf("errore\n");
        }
    }

    PWM_start(pwmHandleBuzzer);
    PWM_setDuty(pwmHandleBuzzer, dutyCycleBuzzer);
}

void startAlarm(void *arg0){
    PWM_setDuty(pwmHandleBuzzer, dutyCycleBuzzer+500);
    printf("buzzer on\n");
}

void stopAlarm(void *arg0){
    PWM_setDuty(pwmHandleBuzzer, dutyCycleBuzzer);
    printf("buzzer off\n");
}


