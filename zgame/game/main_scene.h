#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include "../system/render_system.h"
#include "../system/move_system.h"
#include "../system/input_move_system.h"
#include "../system/bounce_system.h"

#include "../../rizen/rizen.h"

#include "camera_control.h"
#include "day_manager.h"

class MainScene {
public:
    void init(App* app);
    void update();
    
    void begin_render();
    void end_render();

    void render();
    void render_depth(Shader* depth_shader);
    void update_imgui();

    DayManager* day_manager() { return &m_day_manager; }
    Camera* camera() { return m_camera_control.camera(); }

private:
    App* m_app;

    void load();
    void init_entities();

    Renderer* renderer() { return m_app->renderer(); }
    RessourceManager* ressource_manager() { return m_app->ressource_manager(); }
    Renderer2D* renderer2d() { return m_app->renderer_2d(); }

    SkyBox m_skybox;
    Map m_map;
    CameraControl m_camera_control;
    DayManager m_day_manager;

    MoveSystem m_move_system;
    InputMoveSystem m_input_move_system;
    BounceSystem m_bounce_system;
    RenderSystem m_render_system;

    entt::registry m_registry;
    entt::entity m_player;
};

#endif //MAIN_SCENE_H