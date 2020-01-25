#include "SensorRead.h"

SensorRead _bright(BRIGHT_VAL_PIN, 3, 2, true);
SensorRead _speed(SPEED_VAL_PIN, 3);
SensorRead _color(COLOR_VAL_PIN, 3, 2, true);
