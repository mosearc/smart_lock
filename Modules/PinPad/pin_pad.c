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

            printf("insert password\n");

//               string[0]='\0';
//
//                string[0]='I'; string[1]='N'; string[2]='S'; string[3]='R'; string[4]='T';
//                string[5]=' ';
//                string[6]='P'; string[7]='A'; string[8]='S'; string[9]='S'; string[10]='W'; string[11]='O'; string[12]='R'; string[13]='D'; string[14]='\0';
//                Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);

//            sprintf(string, "Tries left: %d", 3-try);
//            Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);

            printf("Tries left: %d", 3-try);

//               string[0]='\0';
//               t=3-try;
//
//                string[0]='T'; string[1]='R'; string[2]='I'; string[3]='E'; string[4]='S';
//                string[5]=' ';
//                string[6]='L'; string[7]='E'; string[8]='F'; string[9]='T'; string[10]=':'; string[11]=t; string[12]=' '; string[13]=' '; string[14]='\0';
//                Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);


            passed = 1;
            digit = 0;
            while(digit < PASSLEN && !timeOut){
                char key = getKey(NULL);
            if(key != '\0') {
                    sprintf(string, "*");
                    Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,15+20*digit,40,OPAQUE_TEXT);

                    printf("*");

//                   string[0]='\0';
//
//                    string[0]='*'; string[1]=' '; string[2]=' '; string[3]=' '; string[4]=' ';
//                    string[5]=' ';
//                    string[6]=' '; string[7]=' '; string[8]=' '; string[9]=' '; string[10]=' '; string[11]=' '; string[12]=' '; string[13]=' '; string[14]='\0';
//                    Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);

                    if(key != correctPass[digit]){
                        passed = 0;
                    }
                    digit++;
                }
            }

            if(passed == 0){
                printf("sbagliato\n");
                sprintf(string, "Wrong password!");
                Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,60,OPAQUE_TEXT);

//                   string[0]='\0';
//
//                    string[0]='w'; string[1]='r'; string[2]='O'; string[3]='N'; string[4]='G';
//                    string[5]=' ';
//                    string[6]='P'; string[7]='A'; string[8]='S'; string[9]='S'; string[10]='W'; string[11]='O'; string[12]='R'; string[13]='D'; string[14]='\0';
//                    Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);


                try++;
                if(try < 3){
                    sprintf(string, "Try again!");
                    Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,70,OPAQUE_TEXT);
                    Graphics_clearDisplay(&g_sContext);


                    printf("try again\n");

//                       string[0]='\0';
//
//                        string[0]='T'; string[1]='R'; string[2]='Y'; string[3]=' '; string[4]=' ';
//                        string[5]=' ';
//                        string[6]='A'; string[7]='G'; string[8]='A'; string[9]='I'; string[10]='N'; string[11]=' '; string[12]=' '; string[13]=' '; string[14]='\0';
//                        Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);
//                                        Graphics_clearDisplay(&g_sContext);


                }else{
                    sprintf(string, "System locked!");
                    Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,70,OPAQUE_TEXT);

                    printf("system locked!\n");

//                       string[0]='\0';
//
//                        string[0]='S'; string[1]='Y'; string[2]='S'; string[3]='T'; string[4]='M';
//                        string[5]=' ';
//                        string[6]='L'; string[7]='O'; string[8]='C'; string[9]='K'; string[10]='E'; string[11]='D'; string[12]=' '; string[13]=' '; string[14]='\0';
//                        Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);


                    failed = 1;
                    Graphics_clearDisplay(&g_sContext);
                }
            }else{
                printf("giusto\n");
                sprintf(string, "Door unlocked!");
                Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,70,OPAQUE_TEXT);
                Graphics_clearDisplay(&g_sContext);

                printf("door unlocked!\n");

//                   string[0]='\0';
//
//                    string[0]='D'; string[1]='O'; string[2]='O'; string[3]='R'; string[4]=' ';
//                    string[5]=' ';
//                    string[6]='U'; string[7]='N'; string[8]='L'; string[9]='O'; string[10]='C'; string[11]='K'; string[12]='E'; string[13]='D'; string[14]='\0';
//                    Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);
//                                    Graphics_clearDisplay(&g_sContext);

            }

        }else{
            sprintf(string, "Insert secret code: ");
            Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,30,OPAQUE_TEXT);

            printf("insert puk\n");

//               string[0]='\0';
//
//                string[0]='I'; string[1]='N'; string[2]='S'; string[3]='E'; string[4]='T';
//                string[5]=' ';
//                string[6]='P'; string[7]='U'; string[8]='K'; string[9]='C'; string[10]='O'; string[11]='D'; string[12]='E'; string[13]=' '; string[14]='\0';
//                Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);


            passed = 1;
            digit = 0;
            while(digit < PASSLEN){
                char key = getKey(NULL);
                if(key != '\0') {
                    sprintf(string, "*");
                    printf("*");
                    Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,20+20*digit,40,OPAQUE_TEXT);

//                       string[0]='\0';
//
//                        string[0]='*';
//                        Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);

                    if(key != secretCode[digit]){
                        passed = 0;
                    }
                    digit++;
                }
            }

            if(passed == 0){
                printf("sbagliato\n");
                sprintf(string, "Wrong code!");
                Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,60,OPAQUE_TEXT);
                Graphics_clearDisplay(&g_sContext);

//                   string[0]='\0';
//
//                    string[0]='W'; string[1]='R'; string[2]='O'; string[3]='N'; string[4]='G';
//                    string[5]=' ';
//                    string[6]='C'; string[7]='O'; string[8]='D'; string[9]='E'; string[10]=' '; string[11]=' '; string[12]=' '; string[13]=' '; string[14]='\0';
//                    Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);
//                                    Graphics_clearDisplay(&g_sContext);


            }else{
                printf("giusto\n System unlocked!\n");
                sprintf(string, "System unlocked!");
                Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,70,OPAQUE_TEXT);
                Graphics_clearDisplay(&g_sContext);

//                   string[0]='\0';
//
//                    string[0]='S'; string[1]='Y'; string[2]='S'; string[3]='T'; string[4]='M';
//                    string[5]=' ';
//                    string[6]='U'; string[7]='N'; string[8]='L'; string[9]='O'; string[10]='C'; string[11]='K'; string[12]='E'; string[13]='D'; string[14]='\0';
//                    Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);
//                                    Graphics_clearDisplay(&g_sContext);

            }
        }
    }
}




