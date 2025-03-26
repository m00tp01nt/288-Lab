
#include <cyBot_Scan.h>
#include <lab/UART-interrupt/uart-interrupt.h>

#include <lab/servo/servo.h>

void scanBetween(cyBOT_Scan_t* scanner, int left, int right, int increment) {

    int i;

    for (i = right; i < left; i += increment)
    {
        cyBOT_Scan(i, scanner);
    }
}

