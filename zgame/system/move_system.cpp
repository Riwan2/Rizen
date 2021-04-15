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
            // if moved face camera direction opposite
            dir_rotation += move.rotation;
            move.dir_rotation = lerp_degrees(move.dir_rotation, dir_rotation, 0.1);
        }


        // std::cout << "left joystick: " << Input::left_controller_dir().x << ";" << Input::left_controller_dir().y << " | ";
        // std::cout << "rigth joystick: " << Input::rigth_controller_dir().x << ";" << Input::rigth_controller_dir().y << "\n";
        // std::cout << "current: " << move.rotation << " | ";
        // std::cout << "desired: " << move.dir_rotation << " | ";
        // std::cout << "final: " << move.rotation + move.dir_rotation << "\n";

        // set movement rotation
        transform.set_rotation(glm::vec3(0, move.rotation, 0));

        // set movement
        velocity = transform.quat * glm::vec3(direction.x, 0, direction.y);
        velocity *= speed;
        move.velocity = velocity;
        transform.move_position(move.velocity);

        // set visual model rotation
        float dir_rotation = move.dir_rotation + move.rotation_offset;

        transform.set_rotation(glm::vec3(0, dir_rotation, 0));
        transform.update();
    }
}

void MoveSystem::update_direction(MoveComponent* move, glm::vec2* direction)
{
        // Game controller
        direction->x = Input::left_controller_axis().x;
        direction->y = Input::left_controller_axis().y;
        
        if (direction->x != 0)
            move->direction.x = Input::left_controller_axis().x;
        if (direction->y != 0)
            move->direction.y = Input::left_controller_axis().y;

        // Keyboard
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

    // Game controller
    rotation = -Input::right_controller_axis().x * 1.2;

    // Keyboard 
    if (Input::key_down(SDLK_RIGHT))
        rotation = -1;
    else if (Input::key_down(SDLK_LEFT))
        rotation = 1;
    
    move->rotation += rotation * move->rotation_speed * Time::game_delta();
}
