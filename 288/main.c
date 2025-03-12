

/**
 * main.c
 */

#include <lab/movement/movement.h>
#include <lab/movement/bump/bump_handlers.h>
#include <lab/lcd/lcd.h>

#include <lab/log/log.h>

#include <lab/oi/open_interface.h>

int main(void)
{
    timer_init();
    lcd_init();
    uart_interrupt_init();

    oi_t *cybot = oi_alloc();
    oi_init(cybot);

    loga("Reset\0");

    move(cybot, 2000, FORWARD, handleBump);

    loga("Done!\0");

    oi_free(cybot);

	return 0;
}
