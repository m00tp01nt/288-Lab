

/**
 * main.c
 */

#include <string.h>
#include <lab/oi/open_interface.h>
#include <cyBot_Scan.h>

#include <lab/UART-interrupt/uart-interrupt.h>

// Lab 01
#include <lab/lcd/lcd.h>

// Lab 02
#include <lab/movement/movement.h>
#include <lab/movement/bump/bump_handlers.h>

// Lab 03
#include <lab/scan/scan.h>
#include <cyBot_Scan.h>

// Lab 04
#include <lab/buttons/button.h>

// Lab 05
#include <lab/log/log.h>

// Lab 06
#include <lab/UART-Interrupt/uart-interrupt.h>

// Lab 08
#include <lab/adc.h>
#include <lab/servo/ir/ir.h>

// Lab 09
#include <lab/servo/ping/ping.h>

// Lab10
#include <lab/servo/servo.h>

int main(void)
{
    char message[50];

    timer_init();
    lcd_init();
    uart_interrupt_init();

    // Want to send this as soon as possible
    loga("\nReset\0");

    button_init();
    adc_init();
    ping_init();
    log_message(PUTTY, "Initialization Complete\0");


    loga("Press Button 4 to start\0");
    while (button_getButton() != 4) {}



    loga("Terminated\0");
    return 0;
}
