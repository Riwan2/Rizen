#include "transform_comp.h"

// update
void TransformComponent::update()
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

// rotation
void TransformComponent::set_rotation(const glm::vec3& rotation)
{
    m_rotation.x = fmod(rotation.x, 360);
    m_rotation.y = fmod(rotation.y, 360);
    m_rotation.z = fmod(rotation.z, 360);
    m_quat = glm::quat(glm::radians(m_rotation));
    m_got_updated = true;
}

void TransformComponent::move_rotation(const glm::vec3& rotation)
{
    m_rotation.x += fmod(rotation.x, 360);
    m_rotation.y += fmod(rotation.y, 360);
    m_rotation.z += fmod(rotation.z, 360);
    m_quat = glm::quat(glm::radians(m_rotation));
    m_got_updated = true;
}

// position
void TransformComponent::set_position(const glm::vec3& position)
{
     m_position = position;
     m_got_updated = true;
}

void TransformComponent::set_position_y(float y)
{ 
    m_position.y = y;
    m_got_updated = true;
}

void TransformComponent::move_position(const glm::vec3& position)
{
    m_position += position;
    m_got_updated = true;
}

// scale
void TransformComponent::set_scale(const glm::vec3& scale)
{
    m_scale = scale;
    m_got_updated = true;
}

void TransformComponent::move_scale(const glm::vec3& scale) {
    m_scale *= scale;
    m_got_updated = true;
}

// quat
void TransformComponent::set_quat(const glm::quat& quat) 
{
    m_quat = quat;
    m_got_updated = true;
}
    
void TransformComponent::multiply_quat(const glm::quat& quat) 
{ 
    m_quat *= quat;
    m_got_updated = true;
}

// up
void TransformComponent::set_up(const glm::vec3& up) { 
    m_up = up; 
    m_up_changed = true; 
    m_got_updated = true; 
}
