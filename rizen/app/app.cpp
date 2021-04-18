#include "app.h"

App::App() {}
App::~App() 
{
    // Imgui
    ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

    // ressource manager
    delete m_ressource_manager;

    // renderer
    delete m_renderer;
    delete m_renderer_2d;

    // input
    Input::clean_up();

    // sdl / opengl
    SDL_GL_DeleteContext(m_gl_context);
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
    m_window = SDL_CreateWindow(info.title, info.pos_x, info.pos_y, info.size_x, info.size_y, info.flags);
    if (m_window == NULL)
        return rizen_error("Window creation error: " + std::string(SDL_GetError()));

    // init context
    m_gl_context = SDL_GL_CreateContext(m_window);

    // init glew
    glewExperimental = GL_TRUE;
    GLenum res = glewInit();
    if (GLEW_OK != res) {
        std::cout << glewGetErrorString(res) << '\n';
        return rizen_error("Glew init error");
    }

    // init input
    Input::init(glm::vec2(info.size_x, info.size_y));
    
    // init math
    init_random();

    // init renderer2D
    m_renderer_2d = new Renderer2D();
    m_renderer_2d->init();

    // init renderer
    m_renderer = new Renderer();
    m_renderer->init();

    // init time
    Time::init();

    // vsync
    SDL_GL_SetSwapInterval(-1);
    
    // Ressource manager
    m_ressource_manager = new RessourceManager(m_renderer);

    // init imgui
    IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(m_window, m_gl_context);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
}

void App::clear(const glm::vec4& color)
{
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void App::begin()
{
    #ifdef APP_DEBUG
    std::cout << "update rate: " << Time::delta() << "ms\n";
    #endif

    Time::update();
    Input::update(m_window);

    glViewport(0, 0, Input::display_size().x, Input::display_size().y);

    ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();
}

void App::end()
{
    ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(m_window);
}