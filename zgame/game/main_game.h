#ifndef MAIN_GAME_H
#define MAIN_GAME_H

#include "../system/render_system.h"
#include "../system/move_system.h"
#include "../system/input_move_system.h"
#include "../system/bounce_system.h"

#include "../../rizen/rizen.h"

#include "day_manager.h"

class MainGame {
public:
    void init(App* app);
    void update(App* app);
private:
    void load(App* app);
    void init_entities(App* app);
    void update_imgui(App* app);

    DayManager m_day_manager;
};

#endif //MAIN_GAME_H