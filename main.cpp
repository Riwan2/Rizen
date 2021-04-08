#include "rizen/rizen.h"

/*
    Init
*/

//int col = 23;
int col = 10;
float s_space = 10.0;
float t_space = 2.0;
float t_f = 1000.0;

int row, nb_t;
Square* tiles;
Shader shader;
Texture texture;

void init()
{
    shader.init("2d/inst_basic.vr", "2d/inst_basic.fa");
    texture.init_jpg("noise.jpg");

    glm::vec2 d_size = Input::display_size();
    float t_size = (d_size.x - s_space * 2) / (col) - t_space;
    row = (d_size.y - s_space * 2) / (t_size + t_space);
    nb_t = row * col;

    tiles = new Square[nb_t];

    std::cout << "row: " << row << ", col: " << col << ", nb:" << nb_t << ", t_size: " << t_size << std::endl;

    for (int i = 0; i < nb_t; i++) {
        int y = i / col;
        int x = i % col;
        Square* t = &tiles[i];
        t->set_size(glm::vec2(t_size, t_size));
        t->set_pos(glm::vec2(x * (t_size + t_space) + s_space, y * (t_size + t_space) + s_space));
        t->update_transform();
        t->color = glm::vec4(1.0, x / (float)col, 0.5, 1.0);
    }
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

    app->renderer_2d()->begin(Input::display_size(), &shader);

    for (int i = 0; i < nb_t; i++) {
       int y = i / col;
       int x = i % col;
       Square* t = &tiles[i];
       t->rotate(0.01 + (y / t_f) + (x / t_f));
       t->update_transform();
       t->update_rotation();
    }

    app->renderer_2d()->render_instanced_squares(tiles, nb_t, &texture);

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
        update(app);
        app->end();
    }

    delete app;
    return 0;
}