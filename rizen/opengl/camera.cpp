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

void Camera::init(const glm::vec2& size) 
{
    m_pos = glm::vec3(0.0, 0.0, 8.0);
    m_target = glm::vec3(0.0);
    m_view = glm::mat4(1.0);
    m_orientation = glm::quat(0.0, 0.0, 0.0, 1.0);
    resize(size);
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

/*
    TPS Camera
*/

CameraTPS::CameraTPS() {}
CameraTPS::~CameraTPS() {}

void CameraTPS::init(const glm::vec2& display_size)
{
    m_angle_around = 90.0f;
	m_angle_y = -20.0f;
	m_roll = 0.0f;
	m_distance = 6.0;
    Camera::init(display_size);
}

void CameraTPS::update()
{
	float horizontal_distance = m_distance * cos(glm::radians(m_angle_y));
	float vertical_distance = m_distance * sin(glm::radians(-m_angle_y));

	m_pos.x = horizontal_distance * cos(glm::radians(m_angle_around)) + m_target.x;
	m_pos.y = vertical_distance + m_target.y;
	m_pos.z = horizontal_distance * sin(glm::radians(m_angle_around)) + m_target.z;

	glm::quat quat_x = glm::angleAxis(glm::radians(-m_angle_around + 90.0f), UP);
	glm::quat quat_y = glm::angleAxis(glm::radians(m_angle_y), RIGHT);
	glm::quat quat_z = glm::angleAxis(glm::radians(m_roll), FRONT);

	m_orientation = quat_x * quat_y * quat_z;
	Camera::update_view();
}
