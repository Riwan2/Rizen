#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>
#include <glm/glm.hpp>

class Input {
public:
    static void init(const glm::vec2& displaySize);
    static void update(SDL_Window* window);

    static bool key_down(SDL_KeyCode key);
    static bool key_pressed(SDL_KeyCode key);
    static glm::vec2 mouse_scroll() { return m_mouse_scroll - m_last_mouse_scroll; }
    static glm::vec2 display_size() { return m_display_size; }

    static bool on_quit() { return m_quit; }
    static bool on_resized() { return m_resized; }

private:
    static Uint8 m_keystates[SDL_NUM_SCANCODES];
    static Uint8 m_last_keystates[SDL_NUM_SCANCODES];

    static glm::vec2 m_mouse_scroll;
    static glm::vec2 m_last_mouse_scroll;
    static glm::vec2 m_display_size;

    static bool m_quit;
    static bool m_resized;

    static void process_event(SDL_Event* event);
    static void window_event(SDL_Window* window);
};

#endif //INPUT_H