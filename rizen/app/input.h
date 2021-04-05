#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>
#include "../utils/glm_utils.h"

class Input {
public:
    static void init(const glm::vec2& displaySize);
    static void update();

    static bool keyDown(SDL_KeyCode key);
    static bool keyPressed(SDL_KeyCode key);
    static glm::vec2 mouseScroll() { return m_mouseScroll - m_lastMouseScroll; }
    static glm::vec2 displaySize() { return m_displaySize; }
    
    static bool onQuit() { return m_quit; }

private:
    static Uint8 m_keystates[SDL_NUM_SCANCODES];
    static Uint8 m_last_keystates[SDL_NUM_SCANCODES];

    static glm::vec2 m_mouseScroll;
    static glm::vec2 m_lastMouseScroll;
    static glm::vec2 m_displaySize;

    static bool m_quit;

    static void processEvent(SDL_Event* event);
};

#endif //INPUT_H