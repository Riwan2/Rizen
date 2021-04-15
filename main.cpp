//#include "zgame/gcomponent.h"
#include "rizen/rizen.h"
#include "zgame/zgame.h"

/*
    Init
*/

// 2D
Shader basic_shader2D;
FrameBuffer frame_buffer;
Square fbuffer_square;

// 3D
CameraTPS camera_tps;
Shader basic_shader, basic_inst_shader;
Texture texture;

// Model
Mesh cube, dragon;
Material material, material2;
Model model, model2, model_dragon;

// System
RenderSystem render_system;
PlayerMoveSystem player_move_system;

// ECS
entt::registry registry;

// Entity
entt::entity player;

/*
    init
*/

void init(App* app)
{
    // Frame buffer
    glm::vec2 d_size = Input::display_size();
    frame_buffer.init(d_size);
    fbuffer_square.set_origin(glm::vec2(0.5));
    fbuffer_square.set_pos(d_size * 0.5f);
    fbuffer_square.set_size(d_size * 0.8f);
    fbuffer_square.update_transform();

    // camera
    camera_tps.init(Input::display_size());
    camera_tps.set_distance(30);
    camera_tps.set_angle_y(30);
    camera_tps.move_target(glm::vec3(0, 8, 0));

    /*
        System
    */

   render_system.init();

    /*
        Shader
    */

    basic_shader2D.init("2d/basic.vr", "2d/basic.fa");
    basic_shader.init("basic.vr", "basic.fa");
    basic_inst_shader.init("inst_basic.vr", "inst_basic.fa");
    
    /*
        Texture
    */

    texture.init_jpg("noise.jpg");

    /*
        Mesh
    */
    
    cube.init("cube.obj");
    dragon.init("dragon.obj");
    
    /* 
        Material
    */
    
    material.init(&basic_shader);
    material.set_color(glm::vec4(1.0, 0.0, 0.0, 0.0));
    material.set_ambient(0.7);
    material.set_reflectivity(1.0);
    material.set_shine_damper(4);
    material.set_texture(&texture);
    render_system.bind_ubo(&material);

    material2.init(&basic_inst_shader);
    material2.set_color(glm::vec4(0, 0, 1, 0));
    material2.set_ambient(0.5);
    render_system.bind_ubo(&material2);

    /*
        Model
    */

    model.init(&cube, &material);
    model_dragon.init(&dragon, &material);
    model2.init_instanced(&cube, &material2);

    /*
        Entity
    */

   player = registry.create();
   {
       registry.emplace<RenderComponent>(player, RenderComponent(&model_dragon));
       registry.emplace<PlayerMoveComponent>(player, PlayerMoveComponent());
       auto transform = &registry.emplace<TransformComponent>(player, TransformComponent());
       
       transform->set_position(glm::vec3(0, 0, 10));
       transform->set_scale(glm::vec3(0.5));
       transform->update();
   }

    for (int i = 0; i < 100; i++) {;
        auto entity = registry.create();
        {
            registry.emplace<RenderComponent>(entity, RenderComponent(&model));
            auto transform = &registry.emplace<TransformComponent>(entity, TransformComponent());

            transform->set_scale(glm::vec3(0.5));

            int x = i % 10;
            int y = i / 10;
            transform->set_position(glm::vec3(x * 2 - 10, y * 2, 0));
            transform->update();
        }
    }

    // for (int i = 0; i < 10000; i++) {
    //     auto entity = registry.create();
    //     {
    //         registry.emplace<Renderable>(entity, Renderable(&model2));
    //         Transform* transform = &registry.emplace<Transform>(entity, Transform());
    //         transform->set_scale(glm::vec3(0.2));

    //         float x = i % 10 + rand_float(-1, 1);
    //         float y = i / 600.0;
    //         float z = rand_float(-7, 7);
    //         transform->set_position(glm::vec3(x * 2 - 10, y, z + 3));
    //         transform->update();
    //     }
    // }
}

/*
    Update
*/

void update(App* app)
{
    // resize window
    if (Input::on_resized()) {
        glm::vec2 d_size = Input::display_size();
        frame_buffer.resize(d_size);
        fbuffer_square.set_pos(d_size * 0.5f);
        fbuffer_square.set_size(d_size * 0.8f);
        fbuffer_square.update_transform();
        camera_tps.resize(d_size);
    }
    
    // update system
    player_move_system.update(registry);

    // update camera
    //camera_tps.move_angle_around(0.1 * Time::game_delta());
    auto player_trans = registry.try_get<TransformComponent>(player);
    auto player_move = registry.try_get<PlayerMoveComponent>(player);

    if (player_move) {
        camera_tps.set_angle_around(-player_move->get_rotation());
    }
    if (player_trans) {
        camera_tps.set_target(player_trans->position);
    }
    
    // set camera angle y with the controller
    float controller_y = Input::right_controller_axis().y * Time::game_delta();

    if (controller_y > 0 && camera_tps.angle_y() < 50)
        camera_tps.move_angle_y(controller_y);
    else if (controller_y < 0 && camera_tps.angle_y() > 20)
        camera_tps.move_angle_y(controller_y);

    camera_tps.update();

    // // render 3D objects
    frame_buffer.bind();
    app->clear(glm::vec4(0.1));
    render_system.render(&camera_tps, registry);
    frame_buffer.unbind();
    
    // // render frame buffer
    app->renderer_2d()->begin(Input::display_size(), &basic_shader2D);
    app->renderer_2d()->render_square(fbuffer_square, frame_buffer.texture());
    app->renderer_2d()->end();
}

int main()
{
    App* app = new App();
    AppInfo info;
    info.size_x = 820;
    info.size_y = 620;
    app->init(info);

    init(app);

    while(!app->on_quit()) {
        app->begin();
        app->clear(glm::vec4(0.2));
        update(app);
        app->end();
    }

    delete app;
    return 0;
}