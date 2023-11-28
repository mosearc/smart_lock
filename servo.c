#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include <stdio.h>


#define PWM_PERIOD 20000   // Timer period for 50 Hz PWM signal
#define PWM_DUTY_CYCLE_0_DEGREE 350   // Adjust based on your servo specifications
#define PWM_DUTY_CYCLE_180_DEGREE 2350   // Adjust based on your servo specifications

void main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Stop watchdog timer

    // Configure GPIO for PWM
    P2->SEL0 |= BIT5;   // Set P2.5 as TimerA0.2 output
    P2->SEL1 &= ~BIT5;
    P2->DIR |= BIT5;

    // Configure TimerA0 for PWM generation
    TIMER_A0->CCR[0] = PWM_PERIOD - 1;   // Set the period
    TIMER_A0->CCTL[2] = OUTMOD_7;        // Set/reset mode

    // Start TimerA0
    TIMER_A0->CTL = TASSEL_2 | MC_1 | TACLR;

    // Main loop
    while (1) {
        // Set the servo to 0 degrees
        TIMER_A0->CCR[2] = PWM_DUTY_CYCLE_0_DEGREE;
        __delay_cycles(1500000);  // Wait for the servo to move

        // Set the servo to 180 degrees
        TIMER_A0->CCR[2] = PWM_DUTY_CYCLE_180_DEGREE;
        __delay_cycles(1500000);  // Wait for the servo to move
    }
}


