

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

    oi_t *cybot = oi_alloc();
    oi_init(cybot);

    loga("Reset");


    move(cybot, 2000, FORWARD, handleBump);

    log_message(LCD, "Done!");

    oi_free(cybot);

	return 0;
}
