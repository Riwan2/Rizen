#ifndef LOG_H
#define LOG_H

#include <iostream>

static bool rizen_error(const std::string& text)
{
    std::cerr << text << '\n';
    return false;
}

#endif //LOG_H