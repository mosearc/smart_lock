#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#include <NoRTOS.h>

/* Driver configuration */
#include <ti/drivers/Board.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
#include "ti_drivers_config.h"

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "uart.h"

#define MAX_LENGHT 64



void configUART(){
    GPIO_init();
    UART_init();

    UART_Params_init(&uartParams);

    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;

    uartHandle = UART_open(CONFIG_UART_0, &uartParams);
    if (uartHandle == NULL) {
        // UART_open() failed
        while (1);
    }
}

void send_UART(char* msg){
    char input='\r';
    UART_write(uartHandle, &input, 1);

    uint8_t i=0;
    while(msg[i]!='\0'){
        UART_write(uartHandle, msg+i, 1);
        ++i;
    }

    input='\n';
    UART_write(uartHandle, &input, 1);
}

bool check_Answer(){

    char answer;
    UART_read(uartHandle, &answer, 1);

    switch(answer){
        case '0':{
            return false;
            break;
        }
        case '1':{
            return true;
            break;
        }
        default:{
            //ERROR
            return false;
            break;
        }
    }

    return false;
}
