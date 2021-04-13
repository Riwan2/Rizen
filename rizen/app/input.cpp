#include "input.h"

Uint8 Input::m_keystates[SDL_NUM_SCANCODES];
Uint8 Input::m_last_keystates[SDL_NUM_SCANCODES];

glm::vec2 Input::m_mouse_scroll;
glm::vec2 Input::m_last_mouse_scroll;
glm::vec2 Input::m_display_size;

SDL_Joystick* Input::m_controller = nullptr;
glm::vec2 Input::m_left_controller_dir;
glm::vec2 Input::m_rigth_controller_dir;

bool Input::m_quit;
bool Input::m_resized;

void Input::init(const glm::vec2& display_size)
{
    memset(m_last_keystates, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
    memcpy(m_keystates, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);

    m_mouse_scroll = glm::vec2(0, 0);
    m_last_mouse_scroll = glm::vec2(0, 0);
    m_display_size = display_size;
    m_quit = false;
    m_resized = false;

    m_left_controller_dir = glm::vec2(0.0);
    m_rigth_controller_dir = glm::vec2(0.0);
    connect_controller();
}

void Input::clean_up()
{
    SDL_JoystickClose(m_controller);
}

void Input::update(SDL_Window* window)
{
    window_event(window);

    m_resized = false;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        Input::process_event(&event);
    }

    memcpy(m_last_keystates, m_keystates, sizeof(Uint8) * SDL_NUM_SCANCODES);
    memcpy(m_keystates, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);

    m_mouse_scroll = glm::vec2(0, 0);
    m_last_mouse_scroll = glm::vec2(0, 0);
}

void Input::window_event(SDL_Window* window)
{
    int wflags = SDL_GetWindowFlags(window);
    if (!(wflags & SDL_WINDOW_INPUT_FOCUS))
        SDL_Delay(40);
}

#include <iostream>

void Input::process_event(SDL_Event* event) 
{
    switch (event->type) {
        case SDL_QUIT:
            m_quit = true;
            break;

        case SDL_MOUSEWHEEL:
            m_last_mouse_scroll = m_mouse_scroll;
            m_mouse_scroll = glm::vec2(event->wheel.x, event->wheel.y);
            break;
        
        case SDL_WINDOWEVENT:
            if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
                m_display_size.x = event->window.data1;
                m_display_size.y = event->window.data2;
                m_resized = true;
            }
            break;

        case SDL_JOYAXISMOTION:
            if (event->jaxis.which == 0) {
                if (event->jaxis.axis == 0) {
                        m_left_controller_dir.x = event->jaxis.value;
                } else if (event->jaxis.axis == 1) {
                        m_left_controller_dir.y = event->jaxis.value;
                } else if (event->jaxis.axis == 3) {
                    if (event->jaxis.value < -CONTROLLER_DEAD_ZONE) {
                        m_rigth_controller_dir.x = -1;
                    } else if (event->jaxis.value > CONTROLLER_DEAD_ZONE) {
                        m_rigth_controller_dir.x = 1;
                    } else {
                        m_rigth_controller_dir.x = 0;
                    }
                } else if (event->jaxis.axis == 4) {
                    if (event->jaxis.value < -CONTROLLER_DEAD_ZONE) {
                        m_rigth_controller_dir.y = -1;
                    } else if (event->jaxis.value > CONTROLLER_DEAD_ZONE) {
                        m_rigth_controller_dir.y = 1;
                    } else {
                        m_rigth_controller_dir.y = 0;
                    }
                }
            }
            break;
        
        default:
            break;
    }
}

bool Input::key_down(SDL_KeyCode keyCode)
{
    SDL_Scancode code = SDL_GetScancodeFromKey(keyCode);
    return m_keystates[code] && m_last_keystates[code];
}

bool Input::key_pressed(SDL_KeyCode keyCode)
{
    SDL_Scancode code = SDL_GetScancodeFromKey(keyCode);
    return m_keystates[code] && !m_last_keystates[code];
}

#include <iostream>


void Input::connect_controller()
{
    std::cout << SDL_NumJoysticks() << std::endl;
    if (SDL_NumJoysticks() > 0) {
        m_controller = SDL_JoystickOpen(0);
        if (m_controller) {
            printf("Opened Joystick 0\n");
            printf("Name: %s\n", SDL_JoystickName(0));
            printf("Number of Axes: %d\n", SDL_JoystickNumAxes(m_controller));
            printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(m_controller));
            printf("Number of Balls: %d\n", SDL_JoystickNumBalls(m_controller));
        }
    }
}