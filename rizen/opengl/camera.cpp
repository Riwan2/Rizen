#include "camera.h"

/*
    Camera
*/

const float FOV = 45;
const float NEAR = 0.1f;
const float FAR = 100.0f;
const glm::vec3 RIGHT = glm::vec3(1.0, 0.0, 0.0);
const glm::vec3 UP = glm::vec3(0.0, 1.0, 0.0);
const glm::vec3 FRONT = glm::vec3(0.0, 0.0, 1.0);

Camera::Camera() {}
Camera::~Camera() {}

bool Camera::init(const glm::vec2& size) 
{
    m_pos = glm::vec3(0.0, 0.0, 8.0);
    m_target = glm::vec3(0.0);
    m_view = glm::mat4(1.0);
    m_orientation = glm::quat(0.0, 0.0, 0.0, 1.0);
    resize(size);
    return true;
}

void Camera::resize(const glm::vec2& size)
{
    m_projection = glm::perspective(glm::radians(FOV), size.x / size.y, NEAR, FAR);
}

void Camera::update()
{
    m_orientation = glm::quatLookAt(glm::normalize(m_target - m_pos), UP);
    update_view();
}

void Camera::update_view()
{
    glm::quat reverse_orient = glm::conjugate(m_orientation);
    m_view = glm::toMat4(reverse_orient) * glm::translate(glm::mat4(1.0), -m_pos);
    m_projection_view = m_projection * m_view;
}