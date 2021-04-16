#include "move_system.h"

void MoveSystem::update(entt::registry& registry)
{
    auto group = registry.group<TransformComponent, RenderComponent, MoveComponent>();
    
    for (auto [entity, transform, render, move] : group.each()) {
        if (glm::length(move.velocity) > 0) {
            transform.set_rotation(move.rotation);
            transform.move_position(move.velocity * (float)Time::game_delta());
        }

        move.quat = glm::angleAxis(glm::radians(move.visual_rotation.y), glm::vec3(0, 1, 0));
        move.quat *= glm::angleAxis(glm::radians(-move.visual_rotation.x), glm::vec3(1, 0, 0));
        move.quat *= glm::angleAxis(glm::radians(move.visual_rotation.z), glm::vec3(0, 0, 1));
        transform.set_quat(move.quat);
    }
}