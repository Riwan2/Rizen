#ifndef RIZEN_MATH_H
#define RIZEN_MATH_H

#include <time.h>
#include <stdlib.h>

/*
    Random
*/

const double PI = 3.141593;
extern long seed;

void init_random();
int rand_int(int min, int max);
float rand_float(float min, float max);

#endif //RIZEN_MATH_H