#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "../../rizen/utils/rizen_math.h"

struct TransformComponent {
    void update()
    {
        model = glm::mat4(1.0);
        model = glm::translate(model, position);
        model = glm::scale(model, scale);
        model *= glm::toMat4(quat);
    }

    // rotation is in degree
    void set_rotation(const glm::vec3& p_rotation)
    {
        rotation.x = glm::radians(fmod(p_rotation.x, 360));
        rotation.y = glm::radians(fmod(p_rotation.y, 360));
        rotation.z = glm::radians(fmod(p_rotation.z, 360));
	    quat = glm::quat(rotation);
    }

    void set_position(const glm::vec3& p_position) { position = p_position; }
    void set_scale(const glm::vec3& p_scale) { scale = p_scale; }

    void move_rotation(const glm::vec3& p_rotation)
    {
        rotation.x += glm::radians(fmod(p_rotation.x, 360));
        rotation.y += glm::radians(fmod(p_rotation.y, 360));
        rotation.z += glm::radians(fmod(p_rotation.z, 360));
	    quat = glm::quat(rotation);
    }

    void move_position(const glm::vec3& p_position) { position += p_position; }
    void move_scale(const glm::vec3& p_scale) { scale *= p_scale; }
    
    glm::vec3 position = glm::vec3(0.0);
    glm::vec3 scale = glm::vec3(1.0);
    glm::vec3 rotation = glm::vec3(0.0);

    glm::quat quat = glm::quat(1.0, 0.0, 0.0, 0.0);
    glm::mat4 model = glm::mat4(1.0);
};

#endif //TRANSFORM_COMPONENT_H