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
std::vector<Entity*> entities;

// Model
Mesh cube;
Material material, material2;
Model model, model2;

class ASystem : public System {
public:
    void update() {
        for (auto entity : m_entities) {
            auto t = Coordinator::get_component<Transform>(entity);
        }  
    }
};

class BSystem : public System {
public:
    void update() {
        for (auto entity : m_entities) {
            auto t = Coordinator::get_component<Transform>(entity);
        }  
    }
};


// System
RenderSystem* render_system;
ASystem* a_system;
BSystem* b_system;

void init(App* app)
{
    /*
        Init Component
    */

    Coordinator::register_component<Transform>();
    Coordinator::register_component<Renderable>();

    /*
        Init System
    */

    render_system = Coordinator::register_system<RenderSystem>();
    {
        Signature signature;
        signature.set(Coordinator::get_component_type<Transform>());
        signature.set(Coordinator::get_component_type<Renderable>());
        Coordinator::set_system_signature<RenderSystem>(signature);
        render_system->init();
    }

    a_system = Coordinator::register_system<ASystem>();
    {
        Signature signature;
        signature.set(Coordinator::get_component_type<Transform>());
        Coordinator::set_system_signature<ASystem>(signature);   
    }

    b_system = Coordinator::register_system<BSystem>();
    {
        Signature signature;
        signature.set(Coordinator::get_component_type<Transform>());
        Coordinator::set_system_signature<BSystem>(signature);   
    }


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

    // Shader
    basic_shader2D.init("2d/basic.vr", "2d/basic.fa");
    basic_shader.init("basic.vr", "basic.fa");
    basic_inst_shader.init("inst_basic.vr", "inst_basic.fa");
    
    // Texture
    texture.init_jpg("noise.jpg");

    // Mesh
    cube.init("cube.obj");
    
    // Material
    material.init(&basic_shader);
    material.set_color(glm::vec4(1.0, 0.0, 0.0, 0.0));
    material.set_ambient(0.7);
    material.set_reflectivity(1.0);
    material.set_shine_damper(4);
    material.set_texture(&texture);
    //app->renderer()->bind_ubo(&material);

    material2.init(&basic_inst_shader);
    material2.set_color(glm::vec4(0, 0, 1, 0));
    material2.set_ambient(0.5);
    //app->renderer()->bind_ubo(&material2);

    // Model
    model.init(&cube, &material);
    model2.init_instanced(&cube, &material2);

    /*
        Entity
    */

   Entity player = Coordinator::create_entity();
   {
       Coordinator::add_component<Transform>(player, Transform());
       Coordinator::add_component<Renderable>(player, Renderable(&model));
       
       auto transform = Coordinator::get_component<Transform>(player);
       transform->set_position(glm::vec3(0, 0, 5));
       transform->set_scale(glm::vec3(2));
       transform->update();
   }

    for (int i = 0; i < 100; i++) {
        Entity entity = Coordinator::create_entity();
        {
            Coordinator::add_component<Transform>(entity, Transform());
            Coordinator::add_component<Renderable>(entity, Renderable(&model));

            auto transform = Coordinator::get_component<Transform>(entity);
            transform->set_scale(glm::vec3(0.5));

            int x = i % 10;
            int y = i / 10;

            transform->set_position(glm::vec3(x * 2 - 10, y * 2, 0));
            transform->update();
        }
    }

    for (int i = 0; i < 5000; i++) {
        Entity entity = Coordinator::create_entity();
        {
            Coordinator::add_component<Transform>(entity, Transform());
            Coordinator::add_component<Renderable>(entity, Renderable(&model2));

            auto transform = Coordinator::get_component<Transform>(entity);
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
    if (Input::on_resized()) {
        glm::vec2 d_size = Input::display_size();
        frame_buffer.resize(d_size);
        fbuffer_square.set_pos(d_size * 0.5f);
        fbuffer_square.set_size(d_size * 0.8f);
        fbuffer_square.update_transform();
        camera_tps.resize(d_size);
    }

    if (Input::key_pressed(SDLK_RETURN)) {
        Input::connect_controller();
    }
    
    // update camera
    camera_tps.move_angle_around(0.5 * Time::game_delta());
    //camera_tps.set_angle_around();
    camera_tps.update();

    // entity update
    // for (auto entity : entities) {
    //     entity->update();
    // }

    // render 3D objects
    frame_buffer.bind();
    app->clear(glm::vec4(0.1));
    // app->renderer()->begin(&camera_tps);
    // app->renderer()->render(entities);
    // app->renderer()->end();

    render_system->render(&camera_tps);
    a_system->update();
    b_system->update();

    frame_buffer.unbind();
    
    // render frame buffer
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

    for (auto entity : entities)
        delete entity;

    delete app;
    return 0;
}