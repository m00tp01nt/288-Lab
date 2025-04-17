#include <lab/servo/servo.h>

void servo_init() {

    // Timer 1B, GPIO PB5
    SYSCTL_RCGCGPIO_R |= 0b10;      // Page 340
    SYSCTL_RCGCTIMER_R |= 0b10;     // Page 405

    GPIO_PORTB_DIR_R |= 0b100000;   // Page 663
    GPIO_PORTB_DEN_R | 0b100000;
    GPIO_PORTB_AFSEL_R |= 0b100000; // Page 672
    GPIO_PORTB_PCTL_R = ((GPIO_PORTB_PCTL_R & ~0xF00000) | 0x700000);   // Page 689

    TIMER1_CTL_R &= ~0b100000000;   // Page 737
    TIMER1_CFG_R = ((TIMER1_CFG_R & ~0b111) | 0b100);   // Page 727
    TIMER1_TBMR_R |= 0b1000;
    TIMER1_TBMR_R &= ~0b100;
    TIMER1_TBMR_R = ((TIMER1_TBMR_R & ~0b11) | 0x2);
    TIMER1_CTL_R &= ~0b100000000000000;

}

void servo_move(uint16_t degrees) {

}


void servo_move_degrees(int degrees) {



}
