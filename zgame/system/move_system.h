#ifndef MOVE_SYSTEM_H
#define MOVE_SYSTEM_H

#include "../component/transform_comp.h"
#include "../component/move_comp.h"
#include "../component/render_comp.h"

#include "../../rizen/utils/rizen_time.h"
#include "../../rizen/app/input.h"

#include "../../extern/entt/entity/registry.hpp"

class MoveSystem {
public:
    void update(entt::registry& registry);
private:
    void update_direction(MoveComponent* move, glm::vec2* direction);
    void update_rotation(MoveComponent* move);
};

#endif //MOVE_SYSTEM_H