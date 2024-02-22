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

#include "mfrc522.h"
#include "nfc.h"

uint8_t cardID[5];

SPI_Params spiParams;
SPI_Handle spiHandle;
SPI_Transaction spiTransaction = {0};

void configNFC(void *arg0)
{
    GPIO_init();
    SPI_init();

    SPI_Params_init(&spiParams);
    spiParams.bitRate = 4000000;
    spiParams.dataSize = 8;
    spiParams.transferMode = SPI_MODE_BLOCKING;

    spiHandle = SPI_open(CONFIG_SPI_0, &spiParams);
    if(spiHandle == NULL){
        while(1);
    }
    MFRC522_Init();
}

bool readCardID(void *arg0, char* buf){
    if(MFRC522_Check(cardID) == MI_OK){
        sprintf(buf, "%x%x%x%x%x", cardID[0], cardID[1], cardID[2], cardID[3], cardID[4]);
        return cardID;
    }

    return false;

}
