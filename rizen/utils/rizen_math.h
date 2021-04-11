#ifndef RIZEN_MATH_H
#define RIZEN_MATH_H

#include <time.h>
#include <stdlib.h>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

/*
    Random
*/

const double PI = 3.141593;
extern long seed;

void init_random();
int rand_int(int min, int max);
float rand_float(float min, float max);

#endif //RIZEN_MATH_H