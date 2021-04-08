#include "rizen_time.h"

/*
    Time
*/

std::chrono::system_clock::time_point Time::m_time_start;
std::chrono::system_clock::time_point Time::m_time_current;
double Time::m_elapsed = 0;
double Time::m_last_elapsed = 0;
double Time::m_delta = 0;

void Time::init() 
{
    m_time_start = std::chrono::system_clock::now();
    update();
}

void Time::update()
{
    m_time_current = std::chrono::system_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(m_time_current - m_time_start);
    m_last_elapsed = m_elapsed;
    m_elapsed = elapsed_time.count();
    m_delta = m_elapsed - m_last_elapsed;
}

const char* Time::str_time() 
{
    double time = Time::time();
    int ms = fmod(time, 100);
    int sec = int(time / 1000) % 60;
    int min = int(time / 60000) % 60;
    int hour = int(time / 3600000) % 60;
    char* str = new char[256];
    sprintf(str, "[%02d:%02d:%02d:%02d]", hour, min, sec, ms);
    return str;
}

/*
    Timer
*/

Timer::Timer() {}
void Timer::init(double time) 
{
    m_duration = time;
    m_time = Time::time() + m_duration;
}

void Timer::reset() 
{
    init(m_duration);
}

bool Timer::flag() 
{
    return Time::time() >= m_time;
}