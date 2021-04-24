#include "camera_control.h"

/*
    Init
*/

void CameraControl::init()
{
    m_camera_tps.init(Input::display_size());
    m_camera_tps.set_distance(100);
    m_camera_tps.set_angle_y(15);
    m_camera_tps.move_target(glm::vec3(0, 8, 0));
    m_camera_tps.set_target(glm::vec3(0, -15, 0));
}

/*
    Resize
*/

void CameraControl::check_resize()
{
    if (!Input::on_resized())
        return;

    glm::vec2 d_size = Input::display_size();
    m_camera_tps.resize(d_size);
}

/*
    Update
*/

void CameraControl::update() 
{
    check_resize();

    glm::vec2 mouse_scroll = Input::mouse_scroll() * (float)Time::game_delta();

    static float x_speed = 0;
    x_speed = lerp(x_speed, x_speed + mouse_scroll.x, 0.3);
    x_speed = lerp(x_speed, 0, 0.1);
    m_camera_tps.move_angle_around(x_speed);

    static float y_speed = 0;
    if (abs(mouse_scroll.x) < 1) {
        y_speed = lerp(y_speed, y_speed + mouse_scroll.y, 0.3);
        y_speed = lerp(y_speed, 0, 0.1);
        float distance = m_camera_tps.distance();
        float distance_max = 120;
        float factor = distance / distance_max + 0.2f;
        float dist_speed = y_speed * factor;

        //std::cout << distance << " speed: " << y_speed << " factor: " << factor << std::endl;
        m_camera_tps.move_distance(dist_speed);

        distance = m_camera_tps.distance();

        if (distance > distance_max)
            m_camera_tps.set_distance(lerp(distance, distance_max, 0.3));
        else if (distance < 0)
           m_camera_tps.set_distance(lerp(distance, 0, 0.3));
    }

    m_camera_tps.update();
}