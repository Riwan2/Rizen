#ifndef APP_H
#define APP_H

#include "../utils/glew_utils.h"
#include "input.h"
#include "../opengl/opengl_2d.h"

struct AppInfo {
    const char* title = "rizen";
    int posX = SDL_WINDOWPOS_CENTERED;
    int posY = SDL_WINDOWPOS_CENTERED;
    int sizeX = 400;
    int sizeY = 400;
    int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
};

class App {
public:
    App();
    ~App();

    bool init(const AppInfo& info);
    void begin();
    void end();
    
    Renderer2D* renderer2D() { return m_renderer2D; }
    bool onQuit() {
        return Input::onQuit() || Input::keyPressed(SDLK_ESCAPE);
    }
    
private:
    SDL_Window* m_window;
    SDL_GLContext m_glContext;
    Renderer2D* m_renderer2D;
};

#endif //APP_H