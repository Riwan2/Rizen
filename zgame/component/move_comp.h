#ifndef MOVE_COMPONENT_H
#define MOVE_COMPONENT_H

struct MoveComponent
{
    MoveComponent() {}

    //speed
    float speed = 0.1;
    float rotation_speed = 1;
    
    // offset but model should be correctly oriented
    float rotation_offset = -90;
    float rotation = 0;

    float get_rotation() { return rotation_offset + rotation; }

    // visual effect to show where player is facing
    float dir_rotation = 0;
    glm::vec2 direction = glm::vec2(0.0);

    // useful for mvoement
    glm::vec3 velocity = glm::vec3(0.0);
};

#endif //MOVE_COMPONENT_H