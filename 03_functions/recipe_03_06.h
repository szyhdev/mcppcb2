#pragma once

#include "recipe_03_common.h"

namespace recipe_03_06
{
using namespace std::string_literals;

template <typename ...Ts>
auto add(Ts... args)
{
    return (... + args);
}

template <typename T, typename ...Ts>
auto add_to(T init, Ts... args)
{
    return (init + ... + args);
}

template <typename ...Ts>
auto all_of(Ts... args)
{
    return (... && args);
}

template <typename ...Ts>
auto any_of(Ts... args)
{
    return (... || args);
}

template <typename ...Ts>
void print_and_count(Ts... args)
{
    int count = 0;
    ((std::cout << args << " ", ++count), ...);
    std::cout << "(" << count << ")" << std::endl;
}

template <typename T>
struct wrapper
{
    T const &value;
};

template <typename T>
constexpr auto operator <(wrapper<T> const &lhs, wrapper<T> const &rhs)
{
    return wrapper<T> {
        (lhs.value < rhs.value) ? lhs.value : rhs.value
    };
}

template <typename ...Ts>
constexpr auto min(Ts &&...args)
{
    return (wrapper<Ts> { args } < ...).value;
}

template <typename ...Ts>
constexpr auto minimum(Ts &&...args)
{
    return (args < ...);
}

void execute()
{
    // fold a parameter pack over a binary operator
    {
        std::cout << add(1) << std::endl;
        std::cout << add(1, 2, 3, 4, 5) << std::endl;
        std::cout << add_to(100, 1, 2, 3, 4, 5) << std::endl;
        std::cout << add("Hello"s) << std::endl;
        std::cout << add("Hello"s, ","s, " "s, "Linux"s) << std::endl;
        std::cout << add_to("Hello"s, ',', ' ', 'L', 'i', 'n', 'u', 'x') << std::endl;
        std::cout << std::endl;
    }

    // all_of, any_of and print_and_count function templates
    {
        std::cout << std::boolalpha << all_of(true, true, true) << ", " <<
                all_of(false, false, false) << ", " <<
                all_of(true, false, true) << std::endl;
        std::cout << std::boolalpha << any_of(true, true, true) << ", " <<
                any_of(false, false, false) << ", " <<
                any_of(true, false, true) << std::endl;
        print_and_count(1, 'a', 3.14, "hello");

        std::cout << std::endl;
    }

    // fold expression that works with binary function
    {
        std::cout << min(5, 3, 4) << std::endl;
        std::cout << std::boolalpha << minimum(5, 3, 4) << std::endl;
    }
}

}
