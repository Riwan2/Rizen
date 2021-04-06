#include "app.h"

App::App() {}
App::~App() 
{
    delete m_renderer2D;
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

bool App::init(const AppInfo& info) 
{
    // init sdl
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        return rizen_error("SDL init error: " + std::string(SDL_GetError()));

    // version - color - buffer
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // init window
    m_window = SDL_CreateWindow(info.title, info.posX, info.posY, info.sizeX, info.sizeY, info.flags);
    if (m_window == NULL)
        return rizen_error("Window creation error: " + std::string(SDL_GetError()));

    // init context
    m_glContext = SDL_GL_CreateContext(m_window);

    // init glew
    glewExperimental = GL_TRUE;
    GLenum res = glewInit();
    if (GLEW_OK != res) {
        std::cout << glewGetErrorString(res) << '\n';
        return rizen_error("Glew init error");
    }

    // init input
    Input::init(glm::vec2(info.sizeX, info.sizeY));

    // init renderer2D
    m_renderer2D = new Renderer2D();
    m_renderer2D->init();

    return true;
}

void App::begin()
{
    Input::update();
    glViewport(0, 0, Input::displaySize().x, Input::displaySize().y);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void App::end()
{
    SDL_GL_SwapWindow(m_window);
}