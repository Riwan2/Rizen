#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "../../rizen/utils/rizen_math.h"

#include <iostream>

const glm::vec3 UP = glm::vec3(0, 1, 0);

class TransformComponent {
public:
    void update()
    {
        if (!m_got_updated) return;
        m_model = glm::mat4(1.0);
        m_model = glm::translate(m_model, m_position);
        m_model = glm::scale(m_model, m_scale);
        if (m_up_changed)
            m_model *= glm::toMat4(rotation_between_vector(glm::vec3(0, 1, 0), m_up));
        m_model *= glm::toMat4(m_quat);
        m_got_updated = false;
    }

    // rotation is in degree
    void set_rotation(const glm::vec3& rotation)
    {
        m_rotation.x = glm::radians(fmod(rotation.x, 360));
        m_rotation.y = glm::radians(fmod(rotation.y, 360));
        m_rotation.z = glm::radians(fmod(rotation.z, 360));
	    m_quat = glm::quat(m_rotation);
        m_got_updated = true;
    }

    void set_position(const glm::vec3& position) { m_position = position; m_got_updated = true; }
    void set_position_y(float y) { m_position.y = y; m_got_updated = true; }
    void set_scale(const glm::vec3& scale) { m_scale = scale; m_got_updated = true; }

    void move_rotation(const glm::vec3& rotation)
    {
        m_rotation.x += glm::radians(fmod(rotation.x, 360));
        m_rotation.y += glm::radians(fmod(rotation.y, 360));
        m_rotation.z += glm::radians(fmod(rotation.z, 360));
	    m_quat = glm::quat(m_rotation);
        m_got_updated = true;
    }

    void move_position(const glm::vec3& position) { m_position += position; m_got_updated = true; }
    void move_scale(const glm::vec3& scale) { m_scale *= scale; m_got_updated = true; }

    void set_quat(const glm::quat& quat) { m_quat = quat; m_got_updated = true; }
    void multiply_quat(const glm::quat& quat) { m_quat *= quat; m_got_updated = true; }

    void set_up(const glm::vec3& up) { m_up = up; m_up_changed = true; m_got_updated = true; }
    const glm::vec3 up() const { return m_up; }

    const glm::vec3& position() const { return m_position; }
    const glm::vec3& scale() const { return m_scale; }
    const glm::vec3& rotation() const { return m_rotation; }

    const glm::quat& quat() const { return m_quat; }
    const glm::mat4& model() const { return m_model; }

private:
    glm::vec3 m_position;
    glm::vec3 m_scale;
    glm::vec3 m_rotation;

    glm::vec3 m_up = UP;

    glm::quat m_quat;
    glm::mat4 m_model;

    bool m_got_updated = false;
    bool m_up_changed = false;
};

#endif //TRANSFORM_COMPONENT_H