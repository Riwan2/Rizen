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

#include <vector>

/*
    Random
*/

const double PI = 3.14159265358979323846264f;
extern int SEED;

void init_random();
void set_seed(int seed);
int rand_int(int min, int max);
float rand_float(float min, float max);

/*
    Random position
*/

/**
 * @brief Search a random position in a square shape. 
 * The position returned doesn't overlap with the positions provided.
 * If there is too much iteration when searching, it return a random position that overlap.
 * @param position middle of the square radius
 * @param size size of the square radius
 * @param radius minimum distance between position
 * @param last_positions list of position to test
 * @return random position
 */
glm::vec2 random_position_no_overlap(const glm::vec2& position, const glm::vec2& size, float radius, const std::vector<glm::vec2>& last_positions);


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
glm::vec4 lerp(const glm::vec4& a, const glm::vec4& b, float factor);
float lerp_degrees(float a, float b, float factor);

/*
    Triangle interpolation
*/

glm::vec3 barry_centric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos);
float barry_centric_height(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos);

#endif //RIZEN_MATH_H