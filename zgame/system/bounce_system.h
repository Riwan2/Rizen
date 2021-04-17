#ifndef BOUNCE_SYSTEM_H
#define BOUNCE_SYSTEM_H

#include "../component/render_comp.h"
#include "../component/transform_comp.h"
#include "../component/move_comp.h"
#include "../component/bounce_comp.h"

#include "../../rizen/world/map.h"
#include "../../rizen/utils/rizen_time.h"

#include "../../extern/entt/entity/registry.hpp"

/*
    Blueprint
*/

static entt::entity bounce_blueprint(entt::registry& registry, Model* model)
{
    entt::entity entity = registry.create();
    registry.emplace<TransformComponent>(entity);
    registry.emplace<RenderComponent>(entity, model);
    registry.emplace<MoveComponent>(entity);
    registry.emplace<BounceComponent>(entity);
    return entity;
}

/*
    System
*/

class BounceSystem {
public:
    void update(entt::registry& registry, Map* map);
private:
    void make_bounce(MoveComponent* move, BounceComponent* bounce, TransformComponent* transform, RenderComponent* render, Map* map);
};

#endif //BOUNCE_SYSTEM_H