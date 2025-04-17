
#ifndef _LOG_H_

#define _LOG_H_

#include <lab/lcd/lcd.h>
#include <lab/UART-interrupt/uart-interrupt.h>
#include <string.h>

typedef enum LOG_DEVICE {
    LCD,
    PUTTY
} LOG_DEVICE;

void log_message(LOG_DEVICE device, char* message);
void log_putty(char* message);
void log_lcd(char* message);
void loga(char* message);

#endif
