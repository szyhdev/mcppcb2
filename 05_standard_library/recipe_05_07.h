#pragma once

#include "recipe_05_common.h"

#include <iostream>
#include <algorithm>
#include <array>
#include <random>
#include <vector>

namespace recipe_05_07
{

void execute()
{
    // assign a value to all the elements of a range
    {
        std::vector<int> v(5);
        std::fill(v.begin(), v.end(), 42);
        recipe_common::print_collection(v, "v: ");

        std::array<int, 5> a;
        std::fill(a.begin(), a.end(), 42);
        recipe_common::print_collection(a, "a: ");
    }

    // assign values to a number of elements of a range
    {
        std::vector<int> v(10);
        std::fill_n(v.begin(), 5, 42);
        recipe_common::print_collection(v, "v: ");
    }

    // assign value returned by a function to the elements of a range
    {
        std::random_device rd {};
        std::mt19937 mt { rd() };
        std::uniform_int_distribution<> ud { 1, 10 };

        std::vector<int> v(5);
        std::generate(v.begin(), v.end(), [&ud, &mt] {
            return ud(mt);
        });
        recipe_common::print_collection(v, "v: ");
    }

    // assign value returned by a function to a number of elements of a range
    {
        std::vector<int> v(5);
        auto i = 1;
        std::generate_n(v.begin(), v.size(), [&i] {
            return i * i++;
        });
        recipe_common::print_collection(v, "v: ");
    }

    // assign sequentially increasing values to the elements of a range
    {
        std::vector<int> v(5);
        std::iota(v.begin(), v.end(), 1);
        recipe_common::print_collection(v, "v: ");
    }
}

}
