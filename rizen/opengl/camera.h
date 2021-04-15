#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

/*
    Camera
*/ 

// PI / 180
const float PI_180 = 57.295779513f;

class Camera {
public:
    Camera();
    ~Camera();

    virtual void init(const glm::vec2& size);
    virtual void update();
    void update_view();
    void resize(const glm::vec2& size);

    void set_position(const glm::vec3& position) { m_pos = position; }
    void set_target(const glm::vec3& target) { m_target = target; }

    void move_position(const glm::vec3& position) { m_pos += position; }
    void move_target(const glm::vec3& target) { m_target += target; }

    const glm::mat4& projection_view() const { return m_projection_view; }

    const glm::vec3& target() const { return m_target; }
    const glm::vec3& position() const { return m_pos; }

protected:
    glm::vec3 m_pos;
    glm::vec3 m_target;
    glm::quat m_orientation;
    
    glm::mat4 m_view;
    glm::mat4 m_projection;
    glm::mat4 m_projection_view;
};

/*
    TPS Camera
*/

class CameraTPS : public Camera {
public:
	CameraTPS();
	~CameraTPS();

    void init(const glm::vec2& display_size) override;
	void update() override;

	// roll, angle_y and angle_around set in degree
    void set_distance(float distance) { m_distance = distance; }
	void set_roll(float roll) { m_roll = fmod(roll, 360.0f); }
	void set_angle_y(float angle) { m_angle_y = fmod(-angle, 360.0f); }
	void set_angle_around(float angle) { m_angle_around = fmod(angle, 360.0f); }
    void set_angle_around_rad(float angle) { m_angle_around = fmod(angle * PI_180, 360.0f); }

    void move_distance(float distance) { m_distance += distance; }
	void move_roll(float roll) { m_roll = fmod(m_roll + roll, 360.0f); }
	void move_angle_y(float angle) { m_angle_y = fmod(m_angle_y - angle, 360.0f); }
	void move_angle_around(float angle) { m_angle_around = fmod(m_angle_around - angle, 360.0f); }

	const float roll() const { return m_roll; }
	const float angle_y() const { return m_angle_y; }
    const float distance() const { return m_distance; }
	const float angle_around() const { return m_angle_around; }

private:
	float m_angle_around;
	float m_angle_y;
	float m_roll;
    float m_distance;
};


#endif //CAMERA_H