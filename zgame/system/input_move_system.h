#ifndef INPUT_MOVE_SYSTEM_H
#define INPUT_MOVE_SYSTEM_H

#include "../component/transform_comp.h"
#include "../component/move_comp.h"
#include "../component/input_move_comp.h"
#include "../component/render_comp.h"

#include "../../rizen/world/map.h"

#include "../../rizen/utils/rizen_time.h"
#include "../../rizen/app/input.h"

#include "../../extern/entt/entity/registry.hpp"

/*
    Input move blueprint
*/

static entt::entity input_move_blueprint(entt::registry& registry, Model* model)
{
    entt::entity entity = registry.create();
    registry.emplace<RenderComponent>(entity, model);
    registry.emplace<MoveComponent>(entity);
    registry.emplace<InputMoveComponent>(entity);
    registry.emplace<TransformComponent>(entity);
    return entity;
}

/*
    Input move system
*/

class InputMoveSystem {
public:
    void update(entt::registry& registry, Map* map);
private:
    void update_direction(MoveComponent* move, InputMoveComponent* input_move);
    void update_rotation(MoveComponent* move, InputMoveComponent* input_move, TransformComponent* transform);
    void rotate_to_direction(MoveComponent* move, InputMoveComponent* input_move);
    void update_movement(MoveComponent* move, InputMoveComponent* input_move, TransformComponent* transform);
    void visual_rotation(MoveComponent* move, InputMoveComponent* input_move, TransformComponent* transform);
};

#endif //INPUT_MOVE_SYSTEM_H