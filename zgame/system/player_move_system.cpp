#include "player_move_system.h"

void PlayerMoveSystem::update(entt::registry& registry)
{
    auto group = registry.group<TransformComponent, RenderComponent, PlayerMoveComponent>();

    for (auto [entity, transform, render, player_move] : group.each()) {
        // move rotation with input and rotate model with the rotation
        update_rotation(&player_move, &transform);
        // move position with input
        update_direction(&player_move); 
        // rotate the entity facing the direction (only in movement component)
        rotate_to_direction(&player_move);
        // move entity with move component value
        update_movement(&player_move, &transform);
        // rotate the model facing the direction + rotation
        visual_rotation(&player_move, &transform);
        // update model matrix
        transform.update();
    }
}

void PlayerMoveSystem::update_direction(PlayerMoveComponent* move)
{
        // Game controller
        move->direction = glm::vec2(0, 0);

        move->direction.x = Input::left_controller_axis().x;
        move->direction.y = Input::left_controller_axis().y;

        // Keyboard
        if (Input::key_down(SDLK_z))
            move->direction.y = -1;
        else if (Input::key_down(SDLK_s))
            move->direction.y = 1;

        if (Input::key_down(SDLK_d))
            move->direction.x = 1;
        else if (Input::key_down(SDLK_q))
            move->direction.x = -1;

        if (glm::length(move->direction) > 0)
            move->direction = glm::normalize(move->direction);
}

void PlayerMoveSystem::update_rotation(PlayerMoveComponent* move, TransformComponent* transform)
{
    float rotation = 0;
    // Game controller
    rotation = -Input::right_controller_axis().x * 1.2;

    // Keyboard 
    if (Input::key_down(SDLK_RIGHT))
        rotation = -1;
    else if (Input::key_down(SDLK_LEFT))
        rotation = 1;
    
    move->rotation += rotation * move->rotation_speed * Time::game_delta();
    move->rotation = fmod(move->rotation, 360.0f);
    transform->set_rotation(glm::vec3(0, move->rotation, 0));
}

void PlayerMoveSystem::rotate_to_direction(PlayerMoveComponent* move)
{
    if (glm::length(move->direction) <= 0) return;
    float dir_rotation = glm::degrees(atan2(-move->direction.x, -move->direction.y));
    // if moved face camera direction opposite
    dir_rotation += move->rotation;
    move->dir_rotation = lerp_degrees(move->dir_rotation, dir_rotation, move->visual_rotation_speed);
}

void PlayerMoveSystem::update_movement(PlayerMoveComponent* move, TransformComponent* transform)
{
    glm::vec3 velocity = move->velocity;
    velocity += transform->quat * glm::vec3(move->direction.x, 0, move->direction.y);

    if (glm::length(move->direction) > 0) {
        float max_speed = move->max_speed * Time::game_delta();
        move->speed = lerp(move->speed, max_speed, move->acceleration);
        velocity *= move->speed; 
    } else {
        velocity.x = lerp(velocity.x, 0, move->friction);
        velocity.z = lerp(velocity.z, 0, move->friction);
    }

    move->velocity = velocity;
    transform->move_position(move->velocity);
}

void PlayerMoveSystem::visual_rotation(PlayerMoveComponent* move, TransformComponent* transform)
{
    float dir_rotation = move->dir_rotation + move->rotation_offset;
    transform->set_rotation(glm::vec3(0, dir_rotation, 0));
}