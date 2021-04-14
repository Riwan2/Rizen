#include "move_system.h"

void MoveSystem::update(entt::registry& registry)
{
    auto group = registry.group<Transform, Renderable, Move>();

    for (auto [entity, transform, render, move] : group.each()) {
        float rnd = rand_float(0.3, 0.7);
        float rot = Time::game_delta() * rnd;
        transform.move_rotation(glm::vec3(cos(rot), rnd * 10, sin(rot)));
        transform.update();
    }
}