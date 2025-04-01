#ifndef SERVO_H_
#define SERVO_H_


#include "cyBot_Scan.h"

typedef struct object {

    int startAngle;
    int endAngle;
    int midpoint;
    float distance;
    float width;

} object;

object findThinnestObject(cyBOT_Scan_t*, int, int, int, int);
float calculateAngularWidth(object);

void initObject(object*);


#endif
