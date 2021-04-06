#include "rizen/rizen.h"

/*
    Init
*/

int col = 30;
float s_space = 10.0;
float t_space = 2.0;

int row, nb_t;
Square** tiles;
Shader shader;

void init()
{
    shader.init("basic.vr", "basic.fa");

    glm::vec2 d_size = Input::displaySize();
    float t_size = (d_size.x - s_space * 2) / (col) - t_space;
    row = (d_size.y - s_space * 2) / (t_size + t_space);
    nb_t = row * col;

    tiles = new Square*[row];
    for (int i = 0; i < row; i++)
        tiles[i] = new Square[col];

    std::cout << "row: " << row << ", col: " << col << ", nb:" << nb_t << ", t_size: " << t_size << std::endl;

    for (int y = 0; y < row; y++) {
        for (int x = 0; x < col; x++) {
            Square* t = &tiles[y][x];
            t->set_size(glm::vec2(t_size, t_size));
            t->set_pos(glm::vec2(x * (t_size + t_space) + s_space, y * (t_size + t_space) + s_space));
            t->update_transform();
            t->color = glm::vec3(1.0, x / (float)col, 0.5);
        }
    }
}

/*
    Update
*/

void update(App* app)
{
    if (Input::keyPressed(SDLK_RETURN)) {
        init();
        return;
    }

    app->renderer2D()->begin(Input::displaySize(), &shader);

    for (int y = 0; y < row; y++)
        for (int x = 0; x < col; x++)
            app->renderer2D()->renderSquare(tiles[y][x]);

    app->renderer2D()->end();
}

int main()
{
    App* app = new App();
    AppInfo info;
    info.sizeX = 820;
    info.sizeY = 620;
    app->init(info);

    init();

    while(!app->onQuit()) {
        app->begin();
        update(app);
        app->end();
    }

    delete app;
    return 0;
}