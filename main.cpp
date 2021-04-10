#include "rizen/rizen.h"

/*
    Init
*/

Shader basic_shader, custom_shader;
Texture texture;
FrameBuffer frame_buffer;

Square fbuffer_square;
Square square;

void init()
{
    basic_shader.init("2d/basic.vr", "2d/basic.fa");
    custom_shader.init("2d/basic.vr", "2d/custom.fa");
    texture.init_jpg("noise.jpg");

    glm::vec2 d_size = Input::display_size();

    square.set_pos(d_size * 0.5f);
    square.set_size(d_size * 0.9f);
    square.set_origin(glm::vec2(0.5));
    square.update_transform();

    frame_buffer.init(d_size);

    fbuffer_square.set_origin(glm::vec2(0.5));
    fbuffer_square.set_pos(d_size * 0.5f);
    fbuffer_square.set_size(d_size * 0.8f);
    fbuffer_square.update_transform();
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
    
    // update square
    //square.rotate(0.01f);
    //square.update_rotated();

    // render quads
    app->renderer_2d()->begin(Input::display_size(), &custom_shader);
    frame_buffer.bind();
    app->clear(glm::vec4(0.1));
    app->renderer_2d()->render_square(square);
    frame_buffer.unbind();
    app->renderer_2d()->end();
    
    // first frame buffer
    app->renderer_2d()->begin(Input::display_size(), &basic_shader);
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