#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "../../rizen/utils/rizen_math.h"

const glm::vec3 UP = glm::vec3(0, 1, 0);

class TransformComponent {
public:
    // transform logic
    void update();

    // rotation
    void set_rotation(const glm::vec3& rotation);
    void move_rotation(const glm::vec3& rotation);

    // position
    void set_position(const glm::vec3& position);
    void set_position_y(float y);
    void move_position(const glm::vec3& position);

    // scale
    void set_scale(const glm::vec3& scale);
    void move_scale(const glm::vec3& scale);

    // quat
    void set_quat(const glm::quat& quat);
    void multiply_quat(const glm::quat& quat);

    // up
    void set_up(const glm::vec3& up);
    
    // getters
    const glm::vec3& position() const { return m_position; }
    const glm::vec3& scale() const { return m_scale; }
    const glm::vec3& rotation() const { return m_rotation; }
    const glm::quat& quat() const { return m_quat; }
    const glm::vec3 up() const { return m_up; }
    const glm::mat4& model() const { return m_model; }

private:
    glm::vec3 m_position = glm::vec3(0.0);
    glm::vec3 m_scale = glm::vec3(1.0);
    glm::vec3 m_rotation = glm::vec3(0.0);
    glm::vec3 m_up = UP;
    
    glm::quat m_quat;
    glm::mat4 m_model = glm::mat4(1.0);

    bool m_got_updated = false;
    bool m_up_changed = false;
};

#endif //TRANSFORM_COMPONENT_H