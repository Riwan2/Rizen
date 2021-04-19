#include "zgame/game/main_game.h"

int main()
{
    App* app = new App();
    AppInfo info;
    info.size_x = 820;
    info.size_y = 620;
    app->init(info);

    MainGame* game = new MainGame();
    game->init(app);

    while(!app->on_quit()) {
        app->begin();
        app->clear(glm::vec4(0.2));
        game->update(app);
        app->end();
    }

    delete game;
    delete app;
    return 0;
}