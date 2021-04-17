#ifndef BOUNCE_COMPONENT_H
#define BOUNCE_COMPONENT_H

#include "../../rizen/utils/rizen_time.h"

struct BounceComponent {
    float speed = 0.1;
    float bounce_speed = 0.6;
    float max_height = 4;
    double offset = 0;
    glm::vec2 direction = glm::vec2(0, 1);
    Timer direction_timer;
};

#endif //BOUNCE_COMPONENT_H
