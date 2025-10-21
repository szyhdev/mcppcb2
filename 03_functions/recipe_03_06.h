#pragma once

#include "recipe_03_common.h"

// #define SHOW_OVERLOADED_FUNCTIONS

namespace recipe_03_06
{

template <typename... Ts>
auto add_l(Ts... args)
{
#ifdef SHOW_OVERLOADED_FUNCTIONS
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    return (... + args);
}

template <typename... Ts>
auto add_to_one_l(Ts... args)
{
#ifdef SHOW_OVERLOADED_FUNCTIONS
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    return (1 + ... + args);
}

template <typename... Ts>
auto add_r(Ts... args)
{
#ifdef SHOW_OVERLOADED_FUNCTIONS
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    return (args + ...);
}

template <typename... Ts>
auto add_to_one_r(Ts... args)
{
#ifdef SHOW_OVERLOADED_FUNCTIONS
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    return (args + ... + 1);
}

template <typename... Ts>
auto all_of(Ts... args)
{
    return (... && args);
}

template <typename... Ts>
auto any_of(Ts... args)
{
    return (... || args);
}

template <typename T>
struct wrapper_min
{
    T const &value;
};

template <typename T>
constexpr auto operator <(wrapper_min<T> const &lhs, wrapper_min<T> const &rhs)
{
    return wrapper_min<T> {
        (lhs.value < rhs.value) ? lhs.value : rhs.value
    };
}

template <typename... Ts>
constexpr auto min(Ts&&... args)
{
    return (wrapper_min<Ts> { args } < ...).value;
}

void execute()
{
    // fold a parameter pack over a binary operator
    {
        std::cout << add_l(1) << std::endl;
        std::cout << add_l(1, 2, 3, 4, 5) << std::endl;
        std::cout << add_to_one_l(1, 2, 3, 4, 5) << std::endl;
        std::cout << add_r(1) << std::endl;
        std::cout << add_r(1, 2, 3, 4, 5) << std::endl;
        std::cout << add_to_one_r(1, 2, 3, 4, 5) << std::endl;
    }

    // an implementation of all_of function template
    {
        std::cout << std::boolalpha << all_of(true, true, true) << std::endl;
        std::cout << all_of(false, false, false) << std::endl;
        std::cout << all_of(true, false, true) << std::endl;
        std::cout << std::endl;
    }

    // an implementation of any_of function template
    {
        std::cout << any_of(true, true, true) << std::endl;
        std::cout << any_of(false, false, false) << std::endl;
        std::cout << any_of(true, false, true) << std::endl;
        std::cout << std::endl;
    }

    // fold expressions that work with arbitrary binary functions
    {
        std::cout << min(10, 3, 4, 8, 5) << std::endl;
    }
}

}
