#include "bounce_system.h"

void BounceSystem::update(entt::registry& registry, Map* map)
{
    auto group = registry.group<TransformComponent, RenderComponent, MoveComponent, BounceComponent>();

    for (auto [entity, transform, render, move, bounce] : group.each()) {
        make_bounce(&move, &bounce, &transform, &render, map);

        transform.set_up(map->get_precise_normal(glm::vec2(transform.position().x, transform.position().z)));

        if (bounce.direction_timer.flag()) {
            bounce.direction.x = rand_float(-1, 1);
            bounce.direction.y = rand_float(-1, 1);
            bounce.direction_timer.init(rand_int(500, 5000));
        }


        if (glm::length(bounce.direction) > 0)
            bounce.direction = glm::normalize(bounce.direction);

        float dir_rotation = glm::degrees(atan2(-bounce.direction.x, -bounce.direction.y));
        move.visual_rotation.y = lerp_degrees(move.visual_rotation.y, dir_rotation, 0.1);      

        move.velocity += (glm::vec3(bounce.direction.x, 0, bounce.direction.y));
        move.velocity *= bounce.speed * 0.1;
    }
}

void BounceSystem::make_bounce(MoveComponent* move, BounceComponent* bounce, TransformComponent* transform, RenderComponent* render, Map* map)
{
    float bounce_speed = 100 / bounce->bounce_speed;
    float time = cos((Time::time() + bounce->offset) / bounce_speed) * 0.5 + 0.5;
    float height = map->get_heigth(glm::vec2(transform->position().x, transform->position().z));
    float y = time * bounce->max_height + height + (render->model->mesh()->height() * transform->scale().y) * 0.35f;
    transform->set_position_y(y);
}