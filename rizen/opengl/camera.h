#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

/*
    Camera
*/ 

class Camera {
public:
    Camera();
    ~Camera();

    bool init(const glm::vec2& size);
    void update();
    void update_view();
    void resize(const glm::vec2& size);

    void set_position(const glm::vec3& position) { m_pos = position; }
    void set_target(const glm::vec3& target) { m_target = target; }

    void move_position(const glm::vec3& position) { m_pos += position; }
    void move_target(const glm::vec3& target) { m_target += target; }

    const glm::mat4& projection_view() const { return m_projection_view; }

    const glm::vec3& target() const { return m_target; }
    const glm::vec3& position() const { return m_pos; }

private:
    glm::vec3 m_pos;
    glm::vec3 m_target;
    glm::quat m_orientation;
    
    glm::mat4 m_view;
    glm::mat4 m_projection;
    glm::mat4 m_projection_view;
};

#endif //CAMERA_H