#ifndef BOUNCE_SYSTEM_H
#define BOUNCE_SYSTEM_H

#include "../component/render_comp.h"
#include "../component/transform_comp.h"
#include "../component/move_comp.h"
#include "../component/bounce_comp.h"

#include "../../rizen/world/map.h"
#include "../../rizen/utils/rizen_time.h"

#include "../../extern/entt/entity/registry.hpp"

class BounceSystem {
public:
    void update(entt::registry& registry, Map* map);
private:
    void make_bounce(MoveComponent* move, BounceComponent* bounce, TransformComponent* transform, RenderComponent* render, Map* map);
};

#endif //BOUNCE_SYSTEM_H