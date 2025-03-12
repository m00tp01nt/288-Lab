#include <lab/movement/bump/bump_handlers.h>

void doNothing(oi_t* sensor_data, BUMP* bumpData, double* sum) {

    return;
}

void handleBump(oi_t* sensor_data, BUMP* bumpData, double* sum) {

    *sum -= move(sensor_data, 15, BACKWARD, doNothing);

    if (*bumpData == BUMP_LEFT) {
        rotate(sensor_data, 90, RIGHT, doNothing);
        move(sensor_data, 25, FORWARD, doNothing);
        rotate(sensor_data, 90, LEFT, doNothing);
    }
    else if (*bumpData == BUMP_RIGHT) {
        rotate(sensor_data, 90, LEFT, doNothing);
        move(sensor_data, 25, FORWARD, doNothing);
        rotate(sensor_data, 90, RIGHT, doNothing);
    }

    oi_setWheels(0, 0);
}
