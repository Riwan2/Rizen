#include "day_manager.h"

void DayManager::update()
{
    m_time = fmod(Time::time_sec(), DAY_TIME);
    m_progress = m_time / DAY_TIME;
    m_hour = m_time / (DAY_TIME / 24);
    m_minute = m_time / (DAY_TIME / 24 / 60);
    m_minute = m_minute % 60;

    float factor = 0;

    if (m_progress >= MORNING && m_progress < MIDI) {
        factor = (m_progress - MORNING) / (MIDI - MORNING);
        m_sky_color = lerp(MORNING_COLOR, MIDI_COLOR, factor);

    } else if (m_progress >= MIDI && m_progress < EVENING) {
        factor = (m_progress - MIDI) / (EVENING - MIDI);
        m_sky_color = lerp(MIDI_COLOR, EVENING_COLOR, factor);

    } else {
        float mpro;
        m_is_night = false;
        
        if (m_progress >= EVENING)
            mpro = m_progress - EVENING;
        else
            mpro = m_progress + (1 - EVENING);

        factor = mpro / (1 - EVENING + MORNING);

        if (factor > 0.5) {
            factor = factor * 2 - 1;
            factor = sin(factor * 2 - 0.1) + 0.1;
            m_sky_color = lerp(NIGHT_COLOR, MORNING_COLOR, factor);

        } else {
            factor = factor * 2;
            m_night_factor = factor;
            factor = sin(factor * 2 - 0.2f) + 0.1f;
            m_sky_color = lerp(EVENING_COLOR, NIGHT_COLOR, factor);

        }
    }

    m_night_factor = lerp(m_night_factor, 0, 0.01);

    /*
        Sun Direction
    */
   
    float time = (m_time / DAY_TIME) * PI * 2;
    //bool is_day = cos(time / 2 - PI / 2) > 0;
    
    if (!m_is_night) {
        m_sun_direction.y = cos(time + PI) * 0.5 + 0.5;
        m_sun_direction.x = lerp(m_sun_direction.x, cos(time / 2), 0.05);
    } else {
        m_sun_direction.x = lerp(m_sun_direction.x, 0, 0.003);
        m_sun_direction.y = lerp(m_sun_direction.y, 0, 0.003);
    }
}

void DayManager::imgui_update()
{
    ImGui::Begin("Time");
    ImGui::Text("Day Time: %i", (int)DAY_TIME);
    ImGui::Text("Current Time: %09f", time());
    ImGui::ProgressBar(progress());
    ImGui::ProgressBar(MORNING, ImVec2(-1, 0), "morning");
    ImGui::ProgressBar(MIDI, ImVec2(-1, 0), "midi");
    ImGui::ProgressBar(EVENING, ImVec2(-1, 0), "evening");
    ImGui::Text("Current Hour: %02i:%02i", hour(), minute());

    ImGui::ColorEdit3("Night Color", glm::value_ptr(NIGHT_COLOR));

    ImGui::Text("Morning time: %f -> %i", MORNING_TIME, int(MORNING_TIME / (DAY_TIME / 24)));
    ImGui::ColorEdit3("Morning Color", glm::value_ptr(MORNING_COLOR));

    ImGui::Text("Midi time: %f -> %i", MIDI_TIME, int(MIDI_TIME / (DAY_TIME / 24)));
    ImGui::ColorEdit3("Midi Color", glm::value_ptr(MIDI_COLOR));

    ImGui::Text("Evening time: %f -> %i", EVENING_TIME, int(EVENING_TIME / (DAY_TIME / 24)));
    ImGui::ColorEdit3("Evening Color", glm::value_ptr(EVENING_COLOR));

    ImGui::ColorButton("SkyColor", ImVec4(sky_color().x, sky_color().y, sky_color().z, 1.0), 0, ImVec2(200, 50));

    ImGui::End();
}