/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping.h"
#include <lab/timer/timer.h>

#include <lab/log/log.h>

// Global shared variables
// Use extern declarations in the header file

volatile uint32_t g_start_time = 0;
volatile uint32_t g_end_time = 0;
volatile enum{LOW, HIGH, DONE} g_state = LOW; // State of ping echo pulse

void ping_init (void)
{

    // YOUR CODE HERE
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3; //enable timer 3
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1; //port b
    while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R1) == 0)   {} //wait for GPIOB
    while ((SYSCTL_PRTIMER_R & 0b1000) == 0)            {}

    // Configure and enable the timer
    GPIO_PORTB_AFSEL_R  &= ~0x08; //enable alt function
    GPIO_PORTB_PCTL_R   &= ~0x0000F000; //clear bits for PB3
    GPIO_PORTB_PCTL_R   |=  0x00007000; //set T3CCP1
    GPIO_PORTB_DEN_R    |=  0x08; //enable PB3
    GPIO_PORTB_DIR_R    |=  0x08; //PB3 output

    // TIMER 3B INIT
    // INPUT EDGE TIME MODE
    // Ensure the timer is disabled
    TIMER3_CTL_R &= ~0x100;

    // Write the GPTM Configuration register with a value of 0x00000004
    TIMER3_CFG_R = 0x00000004;

    // In the GPTM Timer Mode (GPTMTnMR) register
    // write the TnCMR field to 0x1 and the TnMR field to 0x3
    // and select a count direction by programming the TnCDIR bit.
    TIMER3_TBMR_R |=    0b100;
    TIMER3_TBMR_R |=    0b011;
    TIMER3_TBMR_R &= ~0b10000; // 0 counts down

    // Configure the type of event that the timer
    // captures by writing the TnEVENT field of the GPTM Control (GPTMCTL) register.
    TIMER3_CTL_R |= 0b110000000000; // 'both rising and falling edges'

    //If a prescaler is to be used, write the prescale value
    // to the GPTM Timer n Prescale Register (GPTMTnPR)
    TIMER3_TBPR_R = 0xFF;

    // Load the timer start value into the
    // GPTM Timer n Interval Load (GPTMTnILR) register
    TIMER3_TBILR_R = 0xFFFF;

    // Set up the NVIC Registers


    // If interrupts are required, set the
    // CnEIM bit in the GPTM Interrupt Mask (GPTMIMR) register
    TIMER3_IMR_R |= 0b10000000000;
    IntRegister(INT_TIMER3B, TIMER3B_Handler);
    IntMasterEnable();

    // Set the TnEN bit in the GPTM Control (GPTMCTL)
    // register to enable the timer and start counting
    TIMER3_CTL_R |= 0x100;
}

void ping_trigger (void) {
    g_state = LOW;

    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~0b100000000; //disable timer
    TIMER3_IMR_R &= ~0x800; //disable interrupts

    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= ~0x08;

    // YOUR CODE HERE FOR PING TRIGGER/START PULSE

    GPIO_PORTB_DATA_R &= ~0x08; // 0
    timer_waitMicros(10);           // Wait 5 mS
    GPIO_PORTB_DATA_R |= 0x08;  // 1
    timer_waitMicros(10);           // Wait 3uS
    GPIO_PORTB_DATA_R &= ~0x08; // 0

    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R = 0x04; //CCP1 clear

    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= 0x08;
    TIMER3_IMR_R |= 0b100000000;    //re-enable interrupt CCP1
    TIMER3_CTL_R |= 0x800;          //re-enable T3

}

void TIMER3B_Handler(void) {

  // YOUR CODE HERE
  // As needed, go back to review your interrupt handler code for the UART lab.
  // What are the first lines of code in the ISR? Regardless of the device, interrupt handling
  // includes checking the source of the interrupt and clearing the interrupt status bit.
  // Checking the source: test the MIS bit in the MIS register (is the ISR executing
  // because the input capture event happened and interrupts were enabled for that event?
  // Clearing the interrupt: set the ICR bit (so that same event doesn't trigger another interrupt)
  // The rest of the code in the ISR depends on actions needed when the event happens.

    log_message(LCD, "Hello, World!\0");
    while (1) {}
}

float ping_getDistance (void){

    ping_trigger();

    return 0.0;
}
