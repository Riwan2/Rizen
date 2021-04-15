#include "move_system.h"

void MoveSystem::update(entt::registry& registry)
{
    auto group = registry.group<TransformComponent, RenderComponent, MoveComponent>();

    for (auto [entity, transform, render, move] : group.each()) {
        // init movement variable
        float speed = move.speed * Time::game_delta();
        glm::vec2 direction = glm::vec2(0);
        glm::vec3 velocity = glm::vec3(0);
        
        update_rotation(&move);
        update_direction(&move, &direction); 

        // rotate toward entity direction
        if (glm::length(direction) > 0) {
            direction = glm::normalize(direction);
            float dir_rotation = glm::degrees(atan2(-direction.x, -direction.y));
            std::cout << lerp(move.dir_rotation, dir_rotation, 1);
            move.dir_rotation = lerp_degrees(move.dir_rotation, dir_rotation, 0.1);
        }

        std::cout << "current: " << move.rotation << " | ";
        std::cout << "desired: " << move.dir_rotation << " | ";
        std::cout << "final: " << move.rotation + move.dir_rotation << "\n";

        // set movement rotation
        transform.set_rotation(glm::vec3(0, move.rotation, 0));

        // set movement
        velocity = transform.quat * glm::vec3(direction.x, 0, direction.y);
        velocity *= speed;
        move.velocity = velocity;
        transform.move_position(move.velocity);

        // set visual model rotation
        float dir_rotation = move.rotation + move.dir_rotation + move.rotation_offset;
        transform.set_rotation(glm::vec3(0, dir_rotation, 0));
        transform.update();
    }
}

void MoveSystem::update_direction(MoveComponent* move, glm::vec2* direction)
{
    if (Input::key_down(SDLK_z)) {
        move->direction.y = -1;
        direction->y = -1;
    }
    
    else if (Input::key_down(SDLK_s)) {
        move->direction.y = 1;
        direction->y = 1;
    }

    if (Input::key_down(SDLK_d)) {
        move->direction.x = 1;
        direction->x = 1;
    }
    else if (Input::key_down(SDLK_q)) {
        move->direction.x = -1;
        direction->x = -1;
    }
}

void MoveSystem::update_rotation(MoveComponent* move)
{
    float rotation = 0;
    if (Input::key_down(SDLK_RIGHT))
        rotation = -1;
    else if (Input::key_down(SDLK_LEFT))
        rotation = 1;
    
    move->rotation += rotation * move->rotation_speed * Time::game_delta();
}
