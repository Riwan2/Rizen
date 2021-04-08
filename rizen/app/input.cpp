#include "input.h"

Uint8 Input::m_keystates[SDL_NUM_SCANCODES];
Uint8 Input::m_last_keystates[SDL_NUM_SCANCODES];

glm::vec2 Input::m_mouse_scroll;
glm::vec2 Input::m_last_mouse_scroll;
glm::vec2 Input::m_display_size;

bool Input::m_quit;

void Input::init(const glm::vec2& display_size)
{
    memset(m_last_keystates, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
    memcpy(m_keystates, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);

    m_mouse_scroll = glm::vec2(0, 0);
    m_last_mouse_scroll = glm::vec2(0, 0);
    m_display_size = display_size;
    m_quit = false;
}

void Input::update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        Input::process_event(&event);
    }

    memcpy(m_last_keystates, m_keystates, sizeof(Uint8) * SDL_NUM_SCANCODES);
    memcpy(m_keystates, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);

    m_mouse_scroll = glm::vec2(0, 0);
    m_last_mouse_scroll = glm::vec2(0, 0);
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