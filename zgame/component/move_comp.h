#ifndef MOVE_COMP_H
#define MOVE_COMP_H

struct MoveComponent {
    glm::vec3 velocity = glm::vec3(0.0);
    glm::vec3 last_velocity = glm::vec3(0.0);
    glm::vec3 rotation = glm::vec3(0.0);
    glm::vec3 visual_rotation = glm::vec3(0.0);
    glm::quat quat = glm::quat(1.0, 0, 0, 0);
};

#endif //MOVE_COMP_H