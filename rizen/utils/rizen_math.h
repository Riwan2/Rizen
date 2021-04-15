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

/*
    Quaternion
*/

glm::quat rotate_towards(glm::quat& q1, glm::quat& q2, float max_angle);

/*
    Lerp / Degree Lerp
*/

float lerp(float a, float b, float factor);
glm::vec2 lerp(const glm::vec2& a, const glm::vec2& b, float factor);
float lerp_degrees(float a, float b, float factor);

#endif //RIZEN_MATH_H