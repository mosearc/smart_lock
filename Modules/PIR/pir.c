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

#include "pir.h"

void configPIR(void *arg0){
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P6, GPIO_PIN4);
}

bool readPIR(void *arg0){
    return (GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN4) == GPIO_INPUT_PIN_HIGH);
}

