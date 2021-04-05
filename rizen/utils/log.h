#ifndef LOG_H
#define LOG_H

#include <iostream>

inline bool rizen_error(const std::string& text)
{
    std::cerr << text << std::endl;;
    return false;
}

#endif //LOG_H