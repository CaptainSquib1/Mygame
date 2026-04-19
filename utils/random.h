#pragma once

#include "vec.h"


float random_float(); // [0, 1)
double random_double(); // [0, 1)

int randint(int min, int max);

Vec<float> random_vec(); // x and y in [0, 1)
Vec<float> random_unit_vec(); // length(vec) = 1
