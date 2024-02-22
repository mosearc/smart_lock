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

#include "pin_pad.h"
#include "LcdDriver.h"

#define ROWS 4
#define CLMNS 3
#define PASSLEN 6
#define LENGTH 10

// Graphic library context
Graphics_Context g_sContext;

char correctPass[LENGTH] = {'0','0','0','0','0','0'};
char secretCode[LENGTH] = {'1','1','1','1','1','1'};
char string[50];
int t;

char keymap[ROWS][CLMNS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

void configKeyPad(void *arg0){
    //Set columns as outputs
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN1);
    GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN7);
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN7);

    //Set rows as inputs with pull-up resistors
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN0);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P6, GPIO_PIN6);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P6, GPIO_PIN7);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN3);
}

void configGraphic(void *arg0){
    //Initializes display
    Crystalfontz128x128_Init();

    // Set default screen orientation
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    // Initializes graphics context
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    Graphics_clearDisplay(&g_sContext);
}

char getKey(void *arg0){
    int row, col, pressed = 0;

    //Loop through each column
    for(col = 0; col < CLMNS; ++col){
        //Drive the current column low
        switch(col){
            case 0:
                GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN1);
                break;
            case 1:
                GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
                break;
            case 2:
                GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN7);
                break;
        }

        //Check each row
        for(row = 0; row < ROWS; ++row){
            //If the row is low, a button is pressed
            switch(row){
                case 0:
                    if(GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN0) == GPIO_INPUT_PIN_LOW) pressed = 1;
                    while(pressed && (GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN0) == GPIO_INPUT_PIN_LOW));
                    break;
                case 1:
                    if(GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN6) == GPIO_INPUT_PIN_LOW) pressed = 1;
                    while(pressed && (GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN6) == GPIO_INPUT_PIN_LOW));
                    break;
                case 2:
                    if(GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN7) == GPIO_INPUT_PIN_LOW) pressed = 1;
                    while(pressed && (GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN7) == GPIO_INPUT_PIN_LOW));
                    break;
                case 3:
                    if(GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN3) == GPIO_INPUT_PIN_LOW) pressed = 1;
                    while(pressed && (GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN3) == GPIO_INPUT_PIN_LOW));
                    break;
            }

            if(pressed == 1){
                //Return the corresponding key from the keymap
                return keymap[row][col];
            }
        }

        //Release the current column
        switch(col){
            case 0:
                GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN1);
                break;
            case 1:
                GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN5);
                break;
            case 2:
                GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN7);
                break;
        }
    }

    //No button pressed -> returns EOS
    return '\0';
}

void checkPinPad(void *arg0){
    int digit, try = 0;

    while(!failed && !passed && (!timeOut || puk_enabled)){
        if(puk_enabled == 0){
            sprintf(string, "Insert password: ");
            Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,30,OPAQUE_TEXT);

            sprintf(string, "Tries left: %d", 3-try);
            Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);

            passed = 1;
            digit = 0;
            while(digit < PASSLEN && !timeOut){
                char key = getKey(NULL);
            if(key != '\0') {
                    sprintf(string, "*");
                    Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,15+20*digit,40,OPAQUE_TEXT);

                    printf("*");

                    if(key != correctPass[digit]){
                        passed = 0;
                    }
                    digit++;
                }
            }

            if(passed == 0){
                sprintf(string, "Wrong password!");
                Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,60,OPAQUE_TEXT);

                try++;
                if(try < 3){
                    sprintf(string, "Try again!");
                    Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,70,OPAQUE_TEXT);
                    Graphics_clearDisplay(&g_sContext);

                }else{
                    sprintf(string, "System locked!");
                    Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,70,OPAQUE_TEXT);

                    failed = 1;
                    Graphics_clearDisplay(&g_sContext);
                }
            }else{
                sprintf(string, "Door unlocked!");
                Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,70,OPAQUE_TEXT);
                Graphics_clearDisplay(&g_sContext);
            }

        }else{
            sprintf(string, "Insert secret code: ");
            Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,30,OPAQUE_TEXT);

            passed = 1;
            digit = 0;
            while(digit < PASSLEN){
                char key = getKey(NULL);
                if(key != '\0') {
                    sprintf(string, "*");
                    Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,20+20*digit,40,OPAQUE_TEXT);

                    if(key != secretCode[digit]){
                        passed = 0;
                    }
                    digit++;
                }
            }

            if(passed == 0){
                sprintf(string, "Wrong code!  42");
                Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,60,OPAQUE_TEXT);
                Graphics_clearDisplay(&g_sContext);

            }else{
                sprintf(string, "System unlocked!");
                Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,70,OPAQUE_TEXT);
                Graphics_clearDisplay(&g_sContext);
            }
        }
    }
}




