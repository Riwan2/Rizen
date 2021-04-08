#ifndef LOG_H
#define LOG_H

#include <iostream>

inline bool rizen_error(const std::string& text)
{
    std::cerr << text << '\n';
    return false;
}

#endif //LOG_H