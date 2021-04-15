#ifndef PLAYER_MOVE_SYSTEM_H
#define PLAYER_MOVE_SYSTEM_H

#include "../component/transform_comp.h"
#include "../component/player_move_comp.h"
#include "../component/render_comp.h"

#include "../../rizen/utils/rizen_time.h"
#include "../../rizen/app/input.h"

#include "../../extern/entt/entity/registry.hpp"

class PlayerMoveSystem {
public:
    void update(entt::registry& registry);
private:
    void update_direction(PlayerMoveComponent* move);
    void update_rotation(PlayerMoveComponent* move, TransformComponent* transform);
    void rotate_to_direction(PlayerMoveComponent* move);
    void update_movement(PlayerMoveComponent* move, TransformComponent* transform);
    void visual_rotation(PlayerMoveComponent* move, TransformComponent* transform);
};

#endif //PLAYER_MOVE_SYSTEM_H