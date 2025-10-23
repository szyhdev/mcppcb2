#pragma once

#include "../include/recipe_common.h"

inline void print_time(std::string const &prefix = "")
{
    auto now = std::chrono::system_clock::now();
    auto stime = std::chrono::system_clock::to_time_t(now);
    auto ltime = std::localtime(&stime);

    std::cout << prefix << std::put_time(ltime, "%c") << std::endl;
}
