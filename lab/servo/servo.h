#ifndef SERVO_H_
#define SERVO_H_

#include "cyBot_Scan.h"

typedef struct object {

    int startAngle;
    int endAngle;
    int midpoint;
    float distance;

} object;

object scanBetween(cyBOT_Scan_t*, int, int, int, int);
float calculateAngularWidth(object);

#endif
