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

const double PI = 3.14159265358979323846264f;
extern long seed;

void init_random();
int rand_int(int min, int max);
float rand_float(float min, float max);

/*
    Quaternion
*/

glm::quat rotate_towards(glm::quat q1, glm::quat q2, float max_angle);
glm::quat rotation_between_vector(glm::vec3 start, glm::vec3 dest);

/*
    Lerp / Degree Lerp
*/

float lerp(float a, float b, float factor);
glm::vec2 lerp(const glm::vec2& a, const glm::vec2& b, float factor);
glm::vec3 lerp(const glm::vec3& a, const glm::vec3& b, float factor);
float lerp_degrees(float a, float b, float factor);

/*
    Triangle interpolation
*/

glm::vec3 barry_centric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos);
float barry_centric_height(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos);

#endif //RIZEN_MATH_H