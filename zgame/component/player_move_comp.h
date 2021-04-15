#ifndef PLAYER_MOVE_COMPONENT_H
#define PLAYER_MOVE_COMPONENT_H

struct PlayerMoveComponent
{
    PlayerMoveComponent() {}

    //speed
    float max_speed = 0.15;
    float speed = 0;
    float friction = 0.25;
    float acceleration = 0.1;

    // offset but model should be correctly oriented & rotation
    float rotation_offset = -90;
    float rotation = 0;
    float rotation_speed = 0.5;
    
    float get_rotation() { return rotation_offset + rotation; }

    // visual effect to show where player is facing
    float dir_rotation = 0;
    float visual_rotation_speed = 0.1;

    // useful for movement
    glm::vec3 velocity = glm::vec3(0.0);
    glm::vec2 direction = glm::vec2(0.0);    
};

#endif //PLAYER_MOVE_COMPONENT_H