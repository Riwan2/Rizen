#ifndef APP_H
#define APP_H

#include <GL/glew.h>
#include "input.h"

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
    void update();
    
    bool onQuit() {
        return Input::onQuit() || Input::keyPressed(SDLK_ESCAPE);
    }
    
private:
    SDL_Window* m_window;
    SDL_GLContext m_glContext;

    void begin();
    void end();
};

#endif //APP_H