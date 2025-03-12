#include <lab/log/log.h>

void log_message(LOG_DEVICE device, char* message)
{

    if (device == LCD)
    {
        lcd_clear();
        lcd_puts(message);
    }
    else if (device == PUTTY) {}
}

void log_putty(char* message) {
    log_message(PUTTY, message);
}

void log_lcd(char* message) {
    log_message(LCD, message);
}

// Could really screw things up if timing is super important
void loga(char* message)
{
    log_putty(message);
    log_lcd(message);
}
