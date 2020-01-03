#include "SensorRead.h"

SensorRead _speed(SPEED_VAL_PIN, 3, 2, true);
SensorRead _color(COLOR_VAL_PIN, 3);
SensorRead _bright(BRIGHT_VAL_PIN, 3);
SensorRead _battery(BATTERY_VAL_PIN, 12, 0);
