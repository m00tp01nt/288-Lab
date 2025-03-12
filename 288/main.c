

/**
 * main.c
 */

#include <lab/movement/movement.h>
#include <lab/movement/bump/bump_handlers.h>
#include <lab/lcd/lcd.h>
#include <lab/buttons/button.h>

#include <lab/log/log.h>

#include <lab/oi/open_interface.h>

#include <string.h>

int main(void)
{
    timer_init();
    lcd_init();
    button_init();
    uart_interrupt_init();

    loga("Reset\0");

    char message[50];

    while (1) {
        sprintf(message, "%d", button_getButton());
        loga(message);
    }

	return 0;
}
