// #include "gcomponent.h"

// /*
//     Basic component
// */

// MoveComponent::MoveComponent(CameraTPS* camera) 
// {
//     m_camera = camera;
//     m_orientation = 0;
// }
// MoveComponent::~MoveComponent() {}

// const float SPEED = 0.5;
// const float SPEED_SIZE = 0.3;
// const float ROTATION_SPEED = 2;
// const float ACCELERATION = 0.01;
// const float ACCELERATION_SIZE = 0.015;
// const float FRICTION = 0.7;
// const float FRICTION_SIZE = 0.5;

// float speed = 0;
// float speed_size = 0;

// void MoveComponent::update()
// {
//     bool moved = false;
//     float max_speed = SPEED * Time::game_delta();
//     float rotation_speed = ROTATION_SPEED * Time::game_delta();

//     if (Input::key_down(SDLK_z) || Input::left_controller_dir().y < 0) 
//     {
//         moved = true;
//         //speed -= ACCELERATION;
//         //if (speed < -max_speed)
//         //speed = -max_speed;
//         speed = (Input::left_controller_dir().y / 100000.0) * Time::game_delta();
//     } 
//     else if (Input::key_down(SDLK_s) || Input::left_controller_dir().y > 0) 
//     {
//         moved = true;
//         //speed += ACCELERATION;
//         //if (abs(speed) > max_speed)
//            //speed = max_speed;
//         speed = (Input::left_controller_dir().y / 100000.0) * Time::game_delta();
//     }
//     else 
//     {
//         speed *= FRICTION;
//         if (abs(speed) < 0.01)
//             speed = 0;
//     }

//     m_move_vertical = speed;
    
//     float max_speed_size = SPEED_SIZE * Time::game_delta();

//     if (Input::key_down(SDLK_d) || Input::left_controller_dir().x > 0) 
//     {
//         moved = true;
//         //speed_size += ACCELERATION_SIZE;
//         //if (speed_size > max_speed_size)
//            //speed_size = max_speed_size;
//         speed_size = (Input::left_controller_dir().x / 100000.0) * Time::game_delta();
//     } 
//     else if (Input::key_down(SDLK_q) || Input::left_controller_dir().x < 0) 
//     {
//         moved = true;
//         //speed_size -= ACCELERATION_SIZE;
//         //if (speed_size < -max_speed_size)
//         speed_size = (Input::left_controller_dir().x / 100000.0) * Time::game_delta();
//     } 
//     else 
//     {
//         speed_size *= FRICTION_SIZE;
//         if (abs(speed_size) < 0.01)
//             speed_size = 0;
//     }

//     m_move_horizontal = speed_size;

//     if (Input::key_down(SDLK_LEFT) || Input::rigth_controller_dir().x == -1)
//         m_orientation += rotation_speed;
//     else if (Input::key_down(SDLK_RIGHT) || Input::rigth_controller_dir().x == 1)
//         m_orientation -= rotation_speed;

    
//     //std::cout << Input::left_controller_dir().x << "\n";
//     //std::cout << Input::rigth_controller_dir().x << "\n";

//     static float mspeed = 0;

//     if (moved) {
//         entity()->set_rotation(glm::vec3(0, m_orientation, 0));
//         mspeed += 0.01;
//         if (mspeed > 0.5)
//             mspeed = 0.5;
//     } else
//         mspeed *= 0.9;

//     glm::vec3 movement = entity()->transform().quat * glm::vec3(m_move_horizontal, 0, m_move_vertical);
//     movement *= Time::game_delta();

//     entity()->move_position(movement);

//     entity()->update_transform();

//     m_camera->set_target(entity()->position());
//     m_camera->set_angle_around(-m_orientation + 90);
// }