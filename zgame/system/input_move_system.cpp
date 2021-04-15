#include "input_move_system.h"

void InputMoveSystem::update(entt::registry& registry)
{
    auto group = registry.group<TransformComponent, RenderComponent, MoveComponent, InputMoveComponent>();

    for (auto [entity, transform, render, move, player_move] : group.each()) {
        // move rotation with input and rotate model with the rotation
        update_rotation(&move, &player_move, &transform);
        // move position with input
        update_direction(&move, &player_move); 
        // rotate the entity facing the direction (only in movement component)
        rotate_to_direction(&move, &player_move);
        // move entity with move component value
        update_movement(&move, &player_move, &transform);
        // rotate the model facing the direction + rotation
        visual_rotation(&move, &player_move, &transform);
    }
}

void InputMoveSystem::update_direction(MoveComponent* move, InputMoveComponent* input_move)
{
        // Game controller
        input_move->direction = glm::vec2(0, 0);

        glm::vec2 dir = glm::vec2(0);

        input_move->direction.x = Input::left_controller_axis().x;
        input_move->direction.y = Input::left_controller_axis().y;

        // Keyboard
        if (Input::key_down(SDLK_z))
            input_move->direction.y = -1;
        else if (Input::key_down(SDLK_s))
            input_move->direction.y = 1;

        if (Input::key_down(SDLK_d))
            input_move->direction.x = 1;
        else if (Input::key_down(SDLK_q))
            input_move->direction.x = -1;

        if (glm::length(input_move->direction) > 1)
            input_move->direction = glm::normalize(input_move->direction);
}

void InputMoveSystem::update_rotation(MoveComponent* move, InputMoveComponent* input_move, TransformComponent* transform)
{
    float rotation = 0;
    // Game controller
    rotation = -Input::right_controller_axis().x * 2.0;

    // Keyboard 
    if (Input::key_down(SDLK_RIGHT))
        rotation = -1;
    else if (Input::key_down(SDLK_LEFT))
        rotation = 1;
    
    rotation *= input_move->rotation_speed * Time::game_delta();
    move->rotation.y += fmod(rotation, 360.0f);
    input_move->rotation = move->rotation.y;
    transform->set_rotation(move->rotation);
}

void InputMoveSystem::rotate_to_direction(MoveComponent* move, InputMoveComponent* input_move)
{
    if (glm::length(input_move->direction) <= 0) return;
    float dir_rotation = glm::degrees(atan2(-input_move->direction.x, -input_move->direction.y));
    // if moved face camera direction opposite
    dir_rotation += move->rotation.y;
    // slow rotate if entity input_move slowly and inverse
    float rotation_speed = input_move->visual_rotation_speed;
    float off_speed = (1 - glm::length(input_move->direction)) * rotation_speed;
    // apply rotation with rotation speed
    input_move->dir_rotation = lerp_degrees(input_move->dir_rotation, dir_rotation, rotation_speed - off_speed);
}

static Timer timer;
InputMoveSystem::InputMoveSystem(){
timer.init(200);
}

void InputMoveSystem::update_movement(MoveComponent* move, InputMoveComponent* input_move, TransformComponent* transform)
{
    move->last_velocity = move->velocity;

    if (timer.flag())
        move->velocity += transform->quat() * glm::vec3(input_move->direction.x, 0, input_move->direction.y);

    if (Input::key_pressed(SDLK_SPACE)) {
        move->velocity += glm::angleAxis(glm::radians(input_move->dir_rotation), glm::vec3(0, 1, 0)) * glm::vec3(0, 0, -5);
        timer.reset();
    }

    if (!timer.flag()) {
        move->velocity.x = lerp(move->velocity.x, 0, input_move->friction);
        move->velocity.z = lerp(move->velocity.z, 0, input_move->friction);
        return;
    }

    if (glm::length(input_move->direction) > 0) {
       float max_speed = input_move->max_speed;
       input_move->speed = lerp(input_move->speed, max_speed, input_move->acceleration);
       move->velocity *= input_move->speed;
    } else {
       move->velocity.x = lerp(move->velocity.x, 0, input_move->friction);
       move->velocity.z = lerp(move->velocity.z, 0, input_move->friction);
    }
}

void InputMoveSystem::visual_rotation(MoveComponent* move, InputMoveComponent* input_move, TransformComponent* transform)
{
    float dir_rotation = input_move->dir_rotation + input_move->rotation_offset;
    move->visual_rotation.y = dir_rotation;

    float vel_force = glm::length(move->velocity);
    float last_vel_force = glm::length(move->last_velocity);

    //std::cout << Time::delta() << std::endl;

    if (vel_force > 0 && vel_force >= last_vel_force - 0.0001)
       move->visual_rotation.x = lerp(move->visual_rotation.x, 10, 0.05);
    else if (vel_force > 0.001 && vel_force < last_vel_force)
       move->visual_rotation.x = lerp(move->visual_rotation.x, -10, 0.08);
    else
       move->visual_rotation.x = lerp(move->visual_rotation.x, 0, 0.06);
}