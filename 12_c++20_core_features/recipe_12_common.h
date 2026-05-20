#pragma once

#include "../include/recipe_common.h"

namespace recipe_12_common
{

// #define SHOW_DEBUG_INFO

template <typename T>
void print_debug_info(T value)
{
#ifdef SHOW_DEBUG_INFO
    std::cout << value;
    std::cout << std::endl;
#endif
}

template <typename T, typename ...Ts>
void print_debug_info(T head, Ts... rest)
{
#ifdef SHOW_DEBUG_INFO
    std::cout << head;
    print_debug_info(rest...);
#endif
}

template <typename T>
void execute(T &&t)
{
    while (!t.is_done()) {
        t.resume();
    }
};

}
