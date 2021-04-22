#ifndef DAY_MANAGER_H
#define DAY_MANAGER_H

#include "../../rizen/utils/rizen_time.h"
#include "../../rizen/utils/rizen_math.h"

#include "../../extern/imgui/imgui.h"

const float DAY_TIME = 240;

const float MORNING = 0.35f;
const float MIDI = 0.55f;
const float EVENING = 0.75f;

const float MORNING_TIME = DAY_TIME * MORNING;
const float MIDI_TIME = DAY_TIME * MIDI;
const float EVENING_TIME = DAY_TIME * EVENING;

static glm::vec3 MORNING_COLOR = glm::vec3(0.922, 0.757, 0.456);
static glm::vec3 MIDI_COLOR = glm::vec3(0.347, 0.775, 0.907);
static glm::vec3 EVENING_COLOR = glm::vec3(0.672, 0.273, 0.355);
static glm::vec3 NIGHT_COLOR = glm::vec3(0.106, 0.106, 0.289);

class DayManager {
public:
    void update();
    void imgui_update();

    const float time() const { return m_time; }
    const float progress() const { return m_progress; }

    const glm::vec3& sky_color() const { return m_sky_color; }
    const glm::vec3& sun_direction() const { return m_sun_direction; }

    const int hour() const { return m_hour; }
    const int minute() const { return m_minute; }
    const float night_factor() const { return m_night_factor; }
    const bool is_night() const { return m_is_night; }

private:
    float m_time;
    float m_progress;
    int m_hour;
    int m_minute;

    float m_night_factor;
    bool m_is_night;

    glm::vec3 m_sky_color;
    glm::vec3 m_sun_direction = glm::vec3(0.0);
};

#endif //DAY_MANAGER_H