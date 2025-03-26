

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

int main(void)
{
    timer_init();
    lcd_init();
    uart_interrupt_init();

    // Want to send this as soon as possible
    loga("Reset\0");

    cyBOT_init_Scan(0b111);
    cyBOT_Scan_t scan;

    button_init();

    oi_t *cybot = oi_alloc();
    oi_init(cybot);

    adc_init();

    char message[50];

    int i = 0;

    while (1) {
        sprintf(message, "%d", adc_read());
        log_message(LCD, message);
        for (i = 0; i < 100000; i++) {}
    }

    oi_free(cybot);

    loga("Terminated\0");
    return 0;
}
