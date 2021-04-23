#ifndef MAIN_GAME_H
#define MAIN_GAME_H

#include "main_scene.h"

class MainGame {
public:
    void init(App* app);
    void update(App* app);
private:
    void update_imgui(App* app);
};

#endif //MAIN_GAME_H