#pragma once

#include "recipe_05_common.h"

#include <iostream>
#include <algorithm>
#include <vector>

namespace recipe_05_06
{

using namespace std::string_literals;

void execute()
{
    // sort a range
    {
        std::vector<int> v { 3, 13, 5, 8, 1, 2, 1 };
        recipe_common::print_collection(v, "v: ");

        std::sort(v.begin(), v.end());
        recipe_common::print_collection(v, "v: ");

        std::sort(v.begin(), v.end(), std::greater<>());
        recipe_common::print_collection(v, "v: ");
        std::cout << std::endl;
    }

    // sort a range but keeping order of equal elements
    {
        std::vector<recipe_05_common::Task> v {
            { 10, "Task 1"s },
            { 40, "Task 2"s },
            { 25, "Task 3"s },
            { 10, "Task 6"s },
            { 80, "Task 5"s },
            { 10, "Task 4"s },
        };
        recipe_common::print_collection(v, "v: ");

        std::stable_sort(v.begin(), v.end());
        recipe_common::print_collection(v, "v: ");

        std::stable_sort(v.begin(), v.end(), std::greater<>());
        recipe_common::print_collection(v, "v: ");
        std::cout << std::endl;
    }

    // sort a part of a range and leave the rest in an unspecified order
    {
        std::vector<int> v { 3, 13, 5, 8, 1, 2, 1 };
        recipe_common::print_collection(v, "v: ");

        std::partial_sort(v.begin(), v.begin() + 4, v.end());
        recipe_common::print_collection(v, "v: ");

        std::partial_sort(v.begin(), v.begin() + 4, v.end(), std::greater<>());
        recipe_common::print_collection(v, "v: ");
        std::cout << std::endl;
    }

    // sort a part of a range by copying the sorted elements to a second range and
    // leave the original range unchanged
    {
        std::vector<int> v { 3, 13, 5, 8, 1, 2, 1 };
        std::vector<int> vc(v.size());
        recipe_common::print_collection(v, "v: ");

        std::partial_sort_copy(v.begin(), v.end(), vc.begin(), vc.end());
        recipe_common::print_collection(vc, "vc: ");

        std::partial_sort_copy(v.begin(), v.end(), vc.begin(),
                vc.end(), std::greater<>());
        recipe_common::print_collection(vc, "vc: ");
        std::cout << std::endl;
    }

    // sort a range so that the Nth element is the one that would be in that position
    // if the range was completely sorted, and the elements before it are all smaller and
    // the ones after it are all greater, without any guarantee that they are also ordered
    {
        std::vector<int> v { 3, 13, 5, 8, 1, 2, 1 };
        recipe_common::print_collection(v, "v: ");

        std::nth_element(v.begin(), v.begin() + 3, v.end());
        recipe_common::print_collection(v, "v: ");

        std::nth_element(v.begin(), v.begin() + 3, v.end(), std::greater<>());
        recipe_common::print_collection(v, "v: ");
        std::cout << std::endl;
    }

    // check whether a range is sorted
    {
        std::vector<int> v { 1, 1, 2, 3, 5, 8, 13 };
        recipe_common::print_collection(v, "v: ");

        auto sorted = std::is_sorted(v.cbegin(), v.cend());
        std::cout << "v: is_sorted (<) = " << std::boolalpha << sorted << std::endl;

        sorted = std::is_sorted(v.cbegin(), v.cend(), std::greater<>());
        std::cout << "v: is_sorted (>) = " << sorted << std::endl;
        std::cout << std::endl;
    }

    // find a sorted subrange from beginning of a range
    {
        std::vector<int> v { 3, 13, 5, 8, 1, 2, 1 };
        recipe_common::print_collection(v, "v: ");

        auto it = std::is_sorted_until(v.cbegin(), v.cend());
        std::cout << "v: is_sorted until " << std::distance(v.cbegin(), it) << std::endl;
    }
}

}
