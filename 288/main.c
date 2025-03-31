

/**
 * main.c
 */

#include <string.h>
#include <lab/oi/open_interface.h>

// Lab 01
#include <lab/lcd/lcd.h>

// Lab 02
#include <lab/movement/movement.h>
#include <lab/movement/bump/bump_handlers.h>

// Lab 03
#include <lab/servo/servo.h>
#include <cyBot_Scan.h>

// Lab 04
#include <lab/buttons/button.h>

// Lab 05
#include <lab/log/log.h>

// Lab 08
#include <lab/adc.h>
#include <lab/servo/ir/ir.h>

int main(void)
{
    timer_init();
    lcd_init();
    uart_interrupt_init();

    // Want to send this as soon as possible
    loga("\r\nReset\0");

    cyBOT_init_Scan(0b111);
    right_calibration_value = 248500;
    left_calibration_value  = 1193500;
    cyBOT_Scan_t scanner;

    button_init();

    oi_t *cybot = oi_alloc();
    oi_init(cybot);

    adc_init();

    loga("Initialization Complete\0");

    // Button 4 is the go button
    loga("Press Button 4 to start\0");
    while (button_getButton() != 4) {}

    loga("Running\0");


    scanBetween(&scanner, 180, 0, 1, 5);


    oi_free(cybot);

    loga("Terminated\0");
    return 0;
}
