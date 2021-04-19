#include "rizen_math.h"

/*
    Random
*/

int SEED;

void init_random()
{
    SEED = time(NULL);
    srand(SEED);
}

void set_seed(int seed)
{
    SEED = seed;
    srand(SEED);
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

/*
    Random Position
*/

glm::vec2 random_position_no_overlap(const glm::vec2& position, const glm::vec2& size, float radius, const std::vector<glm::vec2>& last_positions)
{
    int iteration = 0;
    glm::vec2 rand_pos;
    bool finished = false;

    do {
        rand_pos.x = rand_float(position.x, size.x);
        rand_pos.y = rand_float(position.y, size.y);
        finished = true;

        for (auto pos : last_positions) {
            if (abs(rand_pos.x - pos.x) < radius)
                finished = false;
            if (abs(rand_pos.y - pos.y) < radius)
                finished = false;
        }

        iteration++;
    } while (!finished && iteration < 10000);

    return rand_pos;
}

/*
    Quaternion
*/

glm::quat rotate_towards(glm::quat q1, glm::quat q2, float max_angle) 
{
	if(max_angle < 0.001f)
		return q1;

	float cos_theta = dot(q1, q2);
	if(cos_theta > 0.9999f)
		return q2;

	if (cos_theta < 0) {
	    q1 = q1 * -1.0f;
	    cos_theta *= -1.0f;
	}

	float angle = acos(cos_theta);
	if (angle < max_angle)
		return q2;

	float ft = max_angle / angle;
	angle = max_angle;
	glm::quat res = (sin((1.0f - ft) * angle) * q1 + sin(ft * angle) * q2) / sin(angle);
	res = glm::normalize(res);
	return res;
}

glm::quat rotation_between_vector(glm::vec3 start, glm::vec3 dest) {
	start = glm::normalize(start);
	dest = glm::normalize(dest);

	float cos_theta = glm::dot(start, dest);
	glm::vec3 rotation_axis;

	if (cos_theta < -1 + 0.001f){
		rotation_axis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (glm::length2(rotation_axis) < 0.01 ) // bad luck, they were parallel, try again!
			rotation_axis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

		rotation_axis = normalize(rotation_axis);
		return glm::angleAxis(glm::radians(180.0f), rotation_axis);
	}

	rotation_axis = cross(start, dest);

	float s = sqrt((1 + cos_theta) * 2);
	float invs = 1 / s;

	return glm::quat (
		s * 0.5f, 
		rotation_axis.x * invs,
		rotation_axis.y * invs,
		rotation_axis.z * invs
	);
}

/*
    Lerp / Degree Lerp
*/

float lerp(float a, float b, float factor) 
{
    if (factor < 0.001f)
        return a;
    if (abs(b - a) < 0.001f)
        return b;
    return a * (1.f - factor) + b * factor;
}

glm::vec2 lerp(const glm::vec2& a, const glm::vec2& b, float factor) 
{
    glm::vec2 result;
    result.x = lerp(a.x, b.x, factor);
    result.y = lerp(a.y, b.y, factor);
    return result;
}

glm::vec3 lerp(const glm::vec3& a, const glm::vec3& b, float factor) 
{
    glm::vec3 result;
    result.x = lerp(a.x, b.x, factor);
    result.y = lerp(a.y, b.y, factor);
    result.z = lerp(a.z, b.z, factor);
    return result;
}

float lerp_degrees(float a, float b, float factor)
{
    float result;
    float diff = b - a;
    
    if (diff < -180.0) {
        // lerp upwards past 360
        b += 360.f;
        result = lerp(a, b, factor);
        if (result >= 360.f)
            result -= 360.f;
    } else if (diff > 180.0) {
        // lerp downwards past 0
        b -= 360.0;
        result = lerp(a, b, factor);
        if (result < 0.f)
            result += 360.f;
    } else {
        // straight lerp
        result = lerp(a, b, factor);
    }

    return result;
}

/*
    Triangle interpolation
*/

#include "iostream"

glm::vec3 barry_centric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos)
{
    float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
    float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
    float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
    float l3 = 1.0f - l1 - l2;
    return l1 * p1 + l2 * p2 + l3 * p3;
}

float barry_centric_height(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos)
{
    return barry_centric(p1, p2, p3, pos).y;
}