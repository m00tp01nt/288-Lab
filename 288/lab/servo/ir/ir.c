
#include <lab/servo/ir/ir.h>
#include <math.h>

float normalizeData(int value) { return (value / (float)4095) * 3.3; }

float calculateDistance(float normalizedData) { return (506.2561 - (65.9771 * log(normalizedData))); }
