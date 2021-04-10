#ifndef RIZEN_TIME_H
#define RIZEN_TIME_H

#include <chrono>
#include <math.h>
#include <stdio.h>

/* 
    Time
*/

class Time {
public:
    static void init();
    static void update();
    static double time() { return m_elapsed; }
    static double delta() { return m_delta; }
    static const char* str_time();

private:
    static std::chrono::steady_clock::time_point m_time_start;
    static std::chrono::steady_clock::time_point m_time_current;
    static double m_elapsed;
    static double m_last_elapsed;
    static double m_delta;
};

/*
    Timer
*/

class Timer {
public:
    Timer();
    void init(double time);
    void reset();
    bool flag();
private:
    double m_time;
    double m_duration;
};

#endif //RIZEN_TIME_H