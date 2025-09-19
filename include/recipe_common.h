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

}
