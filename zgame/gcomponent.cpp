#include "gcomponent.h"

/*
    Basic component
*/

MoveComponent::MoveComponent() {}
MoveComponent::~MoveComponent() {}

const float SPEED = 0.5;

void MoveComponent::update()
{
    float speed = SPEED * Time::game_delta();

    if (Input::key_down(SDLK_z))
        entity()->move_position(glm::vec3(0, 0, -speed));
    else if (Input::key_down(SDLK_s))
        entity()->move_position(glm::vec3(0, 0, speed));

    if (Input::key_down(SDLK_q))
        entity()->move_position(glm::vec3(-speed, 0, 0));
    else if (Input::key_down(SDLK_d))
        entity()->move_position(glm::vec3(speed, 0, 0));

    entity()->update_transform();
}