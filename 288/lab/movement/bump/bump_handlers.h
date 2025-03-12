
#ifndef BUMP_HANDLERS_H_

#define BUMP_HANDLERS_H_

#include <lab/movement/movement.h>


void handleBump(oi_t* sensor_data, BUMP* bumpData, double* sum);
void doNothing(oi_t* sensor_data, BUMP* bumpData, double* sum);

#endif
