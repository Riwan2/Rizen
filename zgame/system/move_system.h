#ifndef MOVE_SYSTEM_H
#define MOVE_SYSTEM_H

#include "../component/transform_comp.h"
#include "../component/move_comp.h"
#include "../component/input_move_comp.h"
#include "../component/render_comp.h"

#include "../../rizen/utils/rizen_time.h"

#include "../../extern/entt/entity/registry.hpp"

class MoveSystem {
public:
    void update(entt::registry& registry);
private:
};


#endif //MOVE_SYSTEM_H