#include "rizen_math.h"

/*
    Random
*/

long seed;

void init_random()
{
    seed = time(NULL);
    srand(seed);
}

int rand_int(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

float rand_float(float min, float max)
{
    float random = (float)rand() / (float)RAND_MAX;
    return random * (max - min) + min;
}