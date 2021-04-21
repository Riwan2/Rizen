#include "day_manager.h"

#include <iostream>

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
        
        if (m_progress > EVENING)
            mpro = m_progress - EVENING;
        else
            mpro = m_progress + (1 - EVENING);

        factor = mpro / (1 - EVENING + MORNING);

        if (factor > 0.5) {
            factor = factor * 2 - 1;
            factor = sin(factor * 2 - PI / 4) + 0.1f;
            m_sky_color = lerp(NIGHT_COLOR, MORNING_COLOR, factor);

        } else {
            factor = factor * 2;
            factor = sin(factor * 2 - 0.2f) + 0.1f;
            m_sky_color = lerp(EVENING_COLOR, NIGHT_COLOR, factor);

        }
    }

    /*
        Sun Direction
    */
   
    float time = m_time * 2 / PI;

    bool is_day = cos(time / 2 - PI / 2) >= 0;
    if (is_day) {
        m_sun_direction.y = cos(time + PI) * 0.5 + 0.5;
        m_sun_direction.x = lerp(m_sun_direction.x, cos(time / 2), 0.05);
    } else {
        m_sun_direction.x = lerp(m_sun_direction.x, 0, 0.005);
    }
}