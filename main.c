

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
    char message[50];

    timer_init();
    lcd_init();
    uart_interrupt_init();

    // Want to send this as soon as possible
    loga("\nReset\0");

    cyBOT_init_Scan(0b111);

//    cyBOT_SERVRO_cal_t calibrator = cyBOT_SERVO_cal(); return 0;

    right_calibration_value = 238000;
    left_calibration_value  = 1167250;
    cyBOT_Scan_t scanner;

    button_init();

    oi_t *cybot = oi_alloc();
    oi_init(cybot);

    adc_init();

//   calibrate_IR(); return 0;

    cyBOT_Scan(90, &scanner);

    loga("Initialization Complete\0");

    // Button 4 is the go button
    loga("Press Button 4 to start\0");
    while (button_getButton() != 4) {}

    while (1) {
        if (command_byte == 1) {
            oi_setWheels(100, 100);
        }
        else if (command_byte == 2) {
            oi_setWheels(-100, -100);
        }
        else if (command_byte == 3) {
            oi_setWheels(100, -100);
        }
        else if (command_byte == 4) {
            oi_setWheels(-100, 100);
        }
        else if (command_byte == 5) {
            oi_setWheels(0, 0);
        }
        else if (command_byte == 6) {
            findThinnestObject(&scanner, 180 - 45, 45, 1, 5);
        }
    }

    loga("Running\0");

    object position;
    initObject(&position);

    object target;

    while (1) {

        log_message(PUTTY, "finding thinnest object");
        target = findThinnestObject(&scanner, 180, 0, 1, 50);

        sprintf(message, "Target is %f cm away", target.distance);
        log_message(PUTTY, message);

        log_message(PUTTY, "Turning to face the object");
        just_turn(cybot, target.midpoint - 90);
        log_message(PUTTY, "Facing the object");
        if (move_dieOnBump(cybot, ((target.distance * 10) - 140), FORWARD)) {
            break;
        }
        else {
            log_message(PUTTY, "Handling bump");
            // Move to another spot, and the loop continues.

            // Move backwards first
            log_message(PUTTY, "Backing up");
            just_move(cybot, -150);
            log_message(PUTTY, "Done backing up");
            int adjustedTargetAngle = target.midpoint - 90;

            if (target.midpoint < 90) { // If the object is on the right
                log_message(PUTTY, "object was on the RIGHT");
                just_turn(cybot, -90 - adjustedTargetAngle);
                just_move(cybot, 500);
                just_turn(cybot, 90);
                just_move(cybot, 300);
            }
            else { // If the object is in the center or the left
                log_message(PUTTY, "object was on the LEFT");
                just_turn(cybot, 90 - adjustedTargetAngle);
                just_move(cybot, 500);
                just_turn(cybot, -90);
                just_move(cybot, 300);
            }
            log_message(PUTTY, "Done handling bump");
        }
        log_message(PUTTY, "...and the loop continues...");
    }

    oi_free(cybot);
    loga("Terminated\0");
    return 0;
}
