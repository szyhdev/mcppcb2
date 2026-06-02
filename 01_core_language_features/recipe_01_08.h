#pragma once

#include "recipe_01_common.h"

namespace recipe_01_08
{

std::vector<int> get_fibonacci_vec()
{
    return std::vector<int> {
        0, 1, 1, 2, 3, 5, 8, 13
    };
}

std::multimap<int, bool> get_fibonacci_map()
{
    return std::multimap<int, bool> {
        { 0, false },
        { 1, true },
        { 1, true },
        { 2, false },
        { 3, true },
        { 5, true },
        { 8, false },
        { 13, true }
    };
}

void print_value(int const value)
{
    std::cout << value << " ";
}

void execute()
{
    auto values = get_fibonacci_vec();

    // traverse a sequence with specific type for elements
    {
        for (size_t i = 0; i < values.size(); ++i) {
            std::cout << values[i] << " ";
        }
        std::cout << std::endl;

        for (auto itr = values.begin(); itr != values.end(); ++itr) {
            std::cout << *itr << " ";
        }
        std::cout << std::endl;

        std::for_each(values.begin(), values.end(), print_value);
        std::cout << std::endl;

        std::for_each(values.begin(), values.end(),
                [] (auto const value) {
                    std::cout << value << " ";
                });
        std::cout << std::endl;

        auto values = get_fibonacci_vec();
        for (int value : values) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }

    // traverse a sequence without specific type for elements,
    // and let the compiler deduce it
    {
        for (auto &&value : get_fibonacci_vec()) {
            std::cout << value << " ";
        }
        std::cout << std::endl;

        for (auto &value : values) {
            value *= 2;
        }

        for (auto const &value : values) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }

    // use structured bindings and decomposition declaration in C++17
    {
        std::cout << std::boolalpha;
        for (auto &&kvp : get_fibonacci_map()) {
            bool flag;
            int value;
            std::tie(value, flag) = kvp;
            std::cout << std::boolalpha << '(' << value << ", " << flag << ") ";
        }
        std::cout << std::endl;

        for (auto &&kvp : get_fibonacci_map()) {
            std::cout << std::boolalpha << '(' << kvp.first << ", " << kvp.second << ") ";
        }
        std::cout << std::endl;

        for (auto &&[value, flag] : get_fibonacci_map()) {
            std::cout << std::boolalpha << '(' << value << ", " << flag << ") ";
        }
        std::cout << std::endl;
    }
}

}
