#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <NoRTOS.h>

// Driver configuration
#include <ti/drivers/Board.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>

#include "ti_drivers_config.h"

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

#include <ti/drivers/UART.h>


#include "Modules/Buzzer/buzzer.h"
#include "Modules/PinPad/LcdDriver.h"
#include "Modules/NFC/nfc.h"
#include "Modules/NFC/mfrc522.h"
#include "Modules/PinPad/pin_pad.h"
#include "Modules/PIR/pir.h"
#include "Modules/Servo/servo.h"
#include "Modules/UART/uart.h"

#define NUM_STATES 6

Timer_Handle timer_timeout;
Timer_Handle timer_state;

Graphics_Context g_sContext;


char string[50];
int i;

typedef enum {
    INIT,
    DOOR_LOCKED,
    PIN_ENABLED,
    SYSTEM_LOCKED,
    DOOR_UNLOCKED,
    DOOR_OPEN
}State_t;

typedef struct{
    State_t state;
    void (*state_function)(void);
} StateMachine_t;

void fn_INIT(void);
void fn_DOOR_LOCKED(void);
void fn_PIN_ENABLED(void);
void fn_SYSTEM_LOCKED(void);
void fn_DOOR_UNLOCKED(void);
void fn_DOOR_OPEN(void);

State_t current_state = INIT;
State_t previous_state = DOOR_OPEN;


StateMachine_t fsm[] = {
                      {INIT, fn_INIT},
                      {DOOR_LOCKED, fn_DOOR_LOCKED},
                      {PIN_ENABLED, fn_PIN_ENABLED},
                      {SYSTEM_LOCKED, fn_SYSTEM_LOCKED},
                      {DOOR_UNLOCKED, fn_DOOR_UNLOCKED},
                      {DOOR_OPEN, fn_DOOR_OPEN}
};

void start_timer(Timer_Handle handle){
    Timer_stop(handle);
    if (Timer_start(handle) == Timer_STATUS_ERROR) {
        /* Failed to start timer */
        while (1) { printf("Errore\n");    }
    }
}

void stop_timer(Timer_Handle handle){
    Timer_stop(handle);
}

void clear(){
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
}

void resetVariables(){
    timeOut = 0;
    failed = 0;
    passed = 0;
    puk_enabled = 0;
}

void fn_INIT(){
    printf("start\n");

    GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN0);
    current_state = DOOR_LOCKED;
}

void concatstr(char* str1, char* str2, char* buf) {
    while(*str1 != '\0') {
        *buf = *str1;
        buf++;
        str1++;
    }

    while(*str2 != '\0') {
        *buf = *str2;
        buf++;
        str2++;
    }

    *buf = '\0';
}

void fn_DOOR_LOCKED(){
    printf("door locked\n");

    closeDoor(NULL);

    Graphics_clearDisplay(&g_sContext);
    sprintf(string, "PORTA BLOCCATA\n");
    Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);

    resetVariables();

//   string[0]='\0';
//
//    Graphics_clearDisplay(&g_sContext);
//    string[0]='P'; string[1]='O'; string[2]='R'; string[3]='T'; string[4]='A';
//    string[5]=' ';
//    string[6]='B'; string[7]='L'; string[8]='O'; string[9]='C'; string[10]='C'; string[11]='A'; string[12]='T'; string[13]='A'; string[14]='\0';
//    Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);
    char card[32];
    if(readCardID(NULL, card)){
        char msg[256];
        concatstr("/card/isauthorized?cardIdPhysical=", card, msg);
        send_UART(msg);

        if(check_Answer()){
            current_state = PIN_ENABLED;
            start_timer(timer_timeout);
        }

    }
}

void fn_PIN_ENABLED(){
    printf("insert pin\n");

    Graphics_clearDisplay(&g_sContext);
    sprintf(string, "PIN ABILITATO\n");
    Graphics_drawStringCentered(&g_sContext,(int8_t *)"PIN ABILITATO\n",AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);

//    string[0]='\0';

//    Graphics_clearDisplay(&g_sContext);
//    string[0]='P'; string[1]='I'; string[2]='N';
//    string[3]=' ';
//    string[4]='A'; string[5]='B'; string[6]='I'; string[7]='L'; string[8]='I'; string[9]='T'; string[10]='A'; string[11]='T'; string[13]='O'; string[14]='\0';
//    Graphics_drawStringCentered(&g_sContext,(int8_t *)string ,AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);

//    printf("%s \n", string);

    checkPinPad(NULL);
    if(failed){
        current_state = SYSTEM_LOCKED;
        //startAlarm(NULL);
        stop_timer(timer_timeout);
        failed = 0;
        puk_enabled = 1;
    }
    if(passed){
        current_state = DOOR_UNLOCKED;
        clear();
        GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN1);
        start_timer(timer_timeout);
    }
}

void fn_SYSTEM_LOCKED(){
    printf("insert puk\n");

    Graphics_clearDisplay(&g_sContext);
    sprintf(string, "PUK ABILITATO\n");
    Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);

//    string[0]='\0';
//
//    string[0]='P'; string[1]='U'; string[2]='K';
//    string[3]=' ';
//    string[4]='R'; string[5]='I'; string[6]='C'; string[7]='H'; string[8]='I'; string[9]='E'; string[10]='S'; string[11]='T'; string[12]='\0';

    checkPinPad(NULL);
    if(passed){
        stopAlarm(NULL);
        current_state = DOOR_LOCKED;
    }
}

void fn_DOOR_UNLOCKED(){
    printf("door unlocked\n");

    Graphics_clearDisplay(&g_sContext);
    sprintf(string, "PORTA SBLOCCATA\n");
    Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);


//       string[0]='\0';
//
//        Graphics_clearDisplay(&g_sContext);
//        string[0]='P'; string[1]='O'; string[2]='R'; string[3]='T'; string[4]='A';
//        string[5]=' ';
//        string[6]='S'; string[7]='L'; string[8]='O'; string[9]='C'; string[10]='C'; string[11]='A'; string[12]='T'; string[13]='A'; string[14]='\0';
//        Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);

    if(readPIR(NULL)){
        current_state = DOOR_OPEN;
        openDoor(NULL);
        start_timer(timer_timeout);
    }
}

void fn_DOOR_OPEN(){
    printf("porta aperta\n");

    Graphics_clearDisplay(&g_sContext);
    sprintf(string, "PORTA APERTA\n");
    Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);

//       string[0]='\0';
//
//        Graphics_clearDisplay(&g_sContext);
//        string[0]='P'; string[1]='O'; string[2]='R'; string[3]='T'; string[4]='A';
//        string[5]=' ';
//        string[6]='A'; string[7]='P'; string[8]='E'; string[9]='R'; string[10]='T'; string[11]='A'; string[12]='T'; string[13]='A'; string[14]='\0';
//        Graphics_drawStringCentered(&g_sContext,(int8_t *)string,AUTO_STRING_LENGTH,64,90,OPAQUE_TEXT);

    if(readPIR(NULL)) start_timer(timer_timeout);
}

void session_expired(Timer_Handle myHandle){
    printf("\nsession expired\n\n");
    resetVariables();
    current_state = DOOR_LOCKED;
    clear();
    GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN0);
    timeOut=1;
}

void change_state(Timer_Handle myHandle){
    /*if(current_state < NUM_STATES ){
        (*fsm[current_state].state_function)();
    }else{
        //serious error
    }*/
}

void configPorts(){
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);

    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
}

void configTimer(){
    Timer_Params params_to;
    Timer_Params params_ts;

    Timer_init();

    Timer_Params_init(&params_to);
    params_to.period = 10000000;                              //10 seconds
    params_to.periodUnits = Timer_PERIOD_US;
    params_to.timerMode = Timer_ONESHOT_CALLBACK;
    params_to.timerCallback = session_expired;

    timer_timeout = Timer_open(CONFIG_TIMER_0, &params_to);

    Timer_Params_init(&params_ts);
    params_ts.period = 1000000;                               //1 seconds
    params_ts.periodUnits = Timer_PERIOD_US;
    params_ts.timerMode = Timer_CONTINUOUS_CALLBACK;
    params_ts.timerCallback = change_state;

    timer_state = Timer_open(CONFIG_TIMER_1, &params_ts);

    if (timer_timeout == NULL) {
        /* Failed to initialized timer */
        printf("t0\n");
        while (1) {}
    }

    if (timer_state == NULL) {
        /* Failed to initialized timer */
        printf("t1\n");
        while (1) {}
    }
}

void configureAll(){
    // Call driver init functions
    Board_init();
    // Start NoRTOS
    NoRTOS_start();
    // Call mainThread function

    configServo(NULL);
    configBuzzer(NULL);
    configNFC(NULL);
    configPIR(NULL);
    configGraphic(NULL);
    configKeyPad(NULL);
    configTimer();
    configPorts();
    configUART();
}

void main(void){
    WDT_A_holdTimer(); // stop watchdog timer
    configureAll();
    start_timer(timer_state);
    //fn_DOOR_LOCKED();
    while(1){
        PCM_gotoLPM0();
        if(current_state < NUM_STATES ){
            (*fsm[current_state].state_function)();
        }else{
            //serious error
        }

    }
}

