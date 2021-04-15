#ifndef INPUT_MOVE_SYSTEM_H
#define INPUT_MOVE_SYSTEM_H

#include "../component/transform_comp.h"
#include "../component/move_comp.h"
#include "../component/input_move_comp.h"
#include "../component/render_comp.h"

#include "../../rizen/utils/rizen_time.h"
#include "../../rizen/app/input.h"

#include "../../extern/entt/entity/registry.hpp"

class InputMoveSystem {
public:
    InputMoveSystem();
    void update(entt::registry& registry);
private:
    void update_direction(MoveComponent* move, InputMoveComponent* input_move);
    void update_rotation(MoveComponent* move, InputMoveComponent* input_move, TransformComponent* transform);
    void rotate_to_direction(MoveComponent* move, InputMoveComponent* input_move);
    void update_movement(MoveComponent* move, InputMoveComponent* input_move, TransformComponent* transform);
    void visual_rotation(MoveComponent* move, InputMoveComponent* input_move, TransformComponent* transform);
};

#endif //INPUT_MOVE_SYSTEM_H