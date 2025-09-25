#pragma once

#include <iostream>

namespace recipe_common
{

template <typename T>
void print_collection(const T &coll, const std::string &prefix = "")
{
    std::cout << prefix;
    for (auto &e : coll) {
        std::cout << e << ' ';
    }
    std::cout << std::endl;
}

template <typename T>
void print_array(T const *const arr, size_t const size,
        const std::string &prefix = "")
{
    std::cout << prefix;
    for (size_t i = 0; i < size; ++i) {
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;
}

}
