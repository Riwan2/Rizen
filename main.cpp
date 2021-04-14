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
Mesh cube;
Material material, material2;
Model model, model2;

// System
RenderSystem render_system;
MoveSystem move_system;

// ECS
entt::registry registry;

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
    camera_tps.set_angle_y(20);
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
    model2.init_instanced(&cube, &material2);

    /*
        Entity
    */

   auto player = registry.create();
   {
       registry.emplace<Renderable>(player, Renderable(&model));
       Transform* transform = &registry.emplace<Transform>(player, Transform());
       
       transform->set_position(glm::vec3(0, 0, 5));
       transform->set_scale(glm::vec3(2));
       transform->update();
   }

    for (int i = 0; i < 100; i++) {;
        auto entity = registry.create();
        {
            registry.emplace<Renderable>(entity, Renderable(&model));
            registry.emplace<Move>(entity);
            Transform* transform = &registry.emplace<Transform>(entity, Transform());

            transform->set_scale(glm::vec3(0.5));

            int x = i % 10;
            int y = i / 10;
            transform->set_position(glm::vec3(x * 2 - 10, y * 2, 0));
            transform->update();
        }
    }

    for (int i = 0; i < 10000; i++) {
        auto entity = registry.create();
        {
            registry.emplace<Renderable>(entity, Renderable(&model2));
            Transform* transform = &registry.emplace<Transform>(entity, Transform());
            transform->set_scale(glm::vec3(0.2));

            float x = i % 10 + rand_float(-1, 1);
            float y = i / 600.0;
            float z = rand_float(-7, 7);
            transform->set_position(glm::vec3(x * 2 - 10, y, z + 3));
            transform->update();
        }
    }
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

    // detect controller
    if (Input::key_pressed(SDLK_RETURN)) {
        Input::connect_controller();
    }
    
    // update system
    move_system.update(registry);

    // update camera
    camera_tps.move_angle_around(0.1 * Time::game_delta());
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