#include "rizen/rizen.h"

/*
    Init
*/

Shader basic_shader2D, custom_shader2D, basic_shader;
Texture texture;
FrameBuffer frame_buffer;

Square fbuffer_square;
Square square;

Camera camera;
Mesh mesh;

void init()
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
    camera.init(Input::display_size());
    basic_shader.init("basic.vr", "basic.fa");
    mesh.init("cube.obj");
}

/*
    Update
*/

void update(App* app)
{
    if (Input::key_pressed(SDLK_RETURN)) {
        init();
        return;
    }
    
    // update camera
    camera.update();

    // render quads
    frame_buffer.bind();
    app->clear(glm::vec4(0.1));    
    app->renderer()->begin(&camera, &basic_shader);

    app->renderer()->render_mesh(&mesh);

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

    init();

    while(!app->on_quit()) {
        app->begin();
        app->clear(glm::vec4(0.2));
        update(app);
        app->end();
    }

    delete app;
    return 0;
}