#include "input.h"

Uint8 Input::m_keystates[SDL_NUM_SCANCODES];
Uint8 Input::m_last_keystates[SDL_NUM_SCANCODES];

glm::vec2 Input::m_mouseScroll;
glm::vec2 Input::m_lastMouseScroll;
glm::vec2 Input::m_displaySize;

bool Input::m_quit;

void Input::init(const glm::vec2& displaySize)
{
    memset(m_last_keystates, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
    memcpy(m_keystates, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);

    m_mouseScroll = glm::vec2(0, 0);
    m_lastMouseScroll = glm::vec2(0, 0);
    m_displaySize = displaySize;
    m_quit = false;
}

void Input::update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        Input::processEvent(&event);
    }

    memcpy(m_last_keystates, m_keystates, sizeof(Uint8) * SDL_NUM_SCANCODES);
    memcpy(m_keystates, SDL_GetKeyboardState(NULL), sizeof(Uint8) * SDL_NUM_SCANCODES);

    m_mouseScroll = glm::vec2(0, 0);
    m_lastMouseScroll = glm::vec2(0, 0);
}

void Input::processEvent(SDL_Event* event) 
{
    switch (event->type) {
        case SDL_QUIT:
            m_quit = true;
            break;

        case SDL_MOUSEWHEEL:
            m_lastMouseScroll = m_mouseScroll;
            m_mouseScroll = glm::vec2(event->wheel.x, event->wheel.y);
            break;
        
        case SDL_WINDOWEVENT:
            if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
                m_displaySize.x = event->window.data1;
                m_displaySize.y = event->window.data2;
            }
            break;
        
        default:
            break;
    }
}

bool Input::keyDown(SDL_KeyCode keyCode)
{
    SDL_Scancode code = SDL_GetScancodeFromKey(keyCode);
    return m_keystates[code] && m_last_keystates[code];
}

bool Input::keyPressed(SDL_KeyCode keyCode)
{
    SDL_Scancode code = SDL_GetScancodeFromKey(keyCode);
    return m_keystates[code] && !m_last_keystates[code];
}