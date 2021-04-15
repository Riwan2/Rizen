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

/*
    Quaternion
*/

glm::quat rotate_towards(glm::quat& q1, glm::quat& q2, float max_angle) {
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

/*
    Lerp / Degree Lerp
*/

float lerp(float a, float b, float factor) {
  return a * (1.f - factor) + b * factor;
}

float lerp_degrees(float a, float b, float factor)
{
    float result;
    float diff = b - a;
    
    if (diff < -180.0) {
        // lerp upwards past 360
        b += 360.f;
        result = lerp(a, b, factor);
        result = fmod(result, 360.0);
    } else if (diff > 180.0) {
        // lerp downwards past 0
        b -= 360.0;
        result = lerp(a, b, factor);
        result = fmod(result, 360.0);
    } else {
        // straight lerp
        result = lerp(a, b, factor);
    }

    return result;
}