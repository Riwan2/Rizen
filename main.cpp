#include "rizen/rizen.h"

/*
    Init
*/

Shader basic_shader2D, custom_shader2D, basic_shader;
Texture texture;
FrameBuffer frame_buffer;

Square fbuffer_square;
Square square;

CameraTPS camera_tps;
Material material, material2;
Mesh mesh;
Model model, model2;
std::vector<Entity*> entities;

void init(App* app)
{
    basic_shader2D.init("2d/basic.vr", "2d/basic.fa");
    custom_shader2D.init("2d/basic.vr", "2d/custom.fa");
    texture.init_jpg("noise.jpg");

    glm::vec2 d_size = Input::display_size();

    square.set_pos(d_size * 0.5f);
    square.set_size(d_size);
    square.set_origin(glm::vec2(0.5));
    square.update_transform();

    frame_buffer.init(d_size);

    fbuffer_square.set_origin(glm::vec2(0.5));
    fbuffer_square.set_pos(d_size * 0.5f);
    fbuffer_square.set_size(d_size * 0.8f);
    fbuffer_square.update_transform();

    /*
        3D
    */

    // camera
    camera_tps.init(Input::display_size());
    camera_tps.set_distance(30);
    camera_tps.set_angle_y(0);
    camera_tps.move_target(glm::vec3(0, 8, 0));

    basic_shader.init("basic.vr", "basic.fa");
    mesh.init("cube.obj");
    
    material.init(&basic_shader);
    material.set_color(glm::vec4(1.0, 0.0, 0.0, 0.0));
    material.set_ambient(0.7);
    material.set_reflectivity(1.0);
    material.set_shine_damper(4);
    material.set_texture(&texture);

    app->renderer()->bind_ubo(&material);

    material2.init(&basic_shader);
    material2.set_color(glm::vec4(0, 0, 1, 0));
    material2.set_ambient(1.0);

    app->renderer()->bind_ubo(&material2);

    model.init(&mesh, &material);
    model2.init(&mesh, &material2);

    for (int i = 0; i < 100; i++) {
        Entity* entity = new Entity();
        entity->init(&model);
        entity->set_scale(glm::vec3(0.5));

        int x = i % 10;
        int y = i / 10;

        entity->set_position(glm::vec3(x * 2 - 10, y * 2, 0));
        entity->update();
        entities.push_back(entity);
    }

    for (int i = 0; i < 10000; i++) {
        Entity* entity = new Entity();
        entity->init(&model2);
        entity->set_scale(glm::vec3(0.2));

        int x = i % 10;
        int y = i / 10;

        entity->set_position(glm::vec3(x * 2 - 10, y * 2, 3));
        entity->update();
        entities.push_back(entity);
    }
}

/*
    Update
*/

void update(App* app)
{
    if (Input::key_pressed(SDLK_RETURN)) {
        init(app);
        return;
    }
    
    // update camera
    //camera_tps.move_angle_around(0.5);
    camera_tps.update();

    // entity update
    //entity.move_rotation(glm::vec3(0.3, 0.35, 0.15));
    //entity.update();

    // render 3D objects
    frame_buffer.bind();
    app->clear(glm::vec4(0.1));    
    app->renderer()->begin(&camera_tps);

    app->renderer()->render(entities);

    app->renderer()->end();
    frame_buffer.unbind();
    
    // first frame buffer
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