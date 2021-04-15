#include "input.h"

Uint8 Input::m_keystates[SDL_NUM_SCANCODES];
Uint8 Input::m_last_keystates[SDL_NUM_SCANCODES];

glm::vec2 Input::m_mouse_scroll;
glm::vec2 Input::m_last_mouse_scroll;
glm::vec2 Input::m_display_size;

SDL_GameController* Input::m_controller;

glm::vec2 Input::m_left_controller_axis;
glm::vec2 Input::m_right_controller_axis;

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

    m_left_controller_axis = glm::vec2(0.0);
    m_right_controller_axis = glm::vec2(0.0);
    connect_controller();
}

#include <iostream>

void Input::clean_up()
{
    if (m_controller)
        SDL_GameControllerClose(m_controller);
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

    update_controller();
}

void Input::window_event(SDL_Window* window)
{
    int wflags = SDL_GetWindowFlags(window);
    if (!(wflags & SDL_WINDOW_INPUT_FOCUS))
        SDL_Delay(40);
}

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

        case SDL_CONTROLLERDEVICEADDED:
            connect_controller();
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

/*
    Controller
*/

void Input::connect_controller()
{
    #ifdef CONTROLLER_ACTIVE
    if (SDL_NumJoysticks() > 0) {
        if (!SDL_IsGameController(0)) { std::cout << "game controller opening error: " << SDL_GetError() << "\n"; return ; }
        m_controller = SDL_GameControllerOpen(0);
        #ifdef CONTROLLER_INFO
        if (m_controller) {
            std::cout << "Valid controller founded: " << SDL_GameControllerName(m_controller) << "\n";
            char* mapping = SDL_GameControllerMapping(m_controller);
            std::cout << "Mapping: " << mapping << "\n";
            SDL_free(mapping);
        }
        #endif
    }
    #endif
}

void Input::update_controller()
{
    if (!m_controller) return;
    
    m_left_controller_axis.x = SDL_GameControllerGetAxis(m_controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX);
    m_left_controller_axis.y = SDL_GameControllerGetAxis(m_controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY);
    m_right_controller_axis.x = SDL_GameControllerGetAxis(m_controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX);
    m_right_controller_axis.y = SDL_GameControllerGetAxis(m_controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTY);

    m_left_controller_axis /= 32767.0f;
    m_right_controller_axis /= 32767.0f;
}