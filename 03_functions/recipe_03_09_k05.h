#pragma once

#include "recipe_03_09_common.h"

namespace recipe_03_09_k05
{

using namespace recipe_03_09_common;

void execute()
{
    std::array ar = { 3, 2, 7, 4, 9, 1, 5, 8, 6 };
    std::vector<int> nums;
    nums.assign(ar.begin(), ar.end());

    // function
    {
        std::cout << add(2, 3) << std::endl;
        std::cout << std::endl;
    }

    // function pointer
    {
        int (*fp)(int, int) = &add;
        std::cout << fp(2, 3) << std::endl;
        std::cout << calc(fp, 2, 3) << std::endl;

        fp = &sub;
        std::cout << calc(fp, 2, 3) << std::endl;
        std::cout << std::endl;
    }

    // functor (or function object)
    {
        IntAdd ia;
        std::cout << ia(2, 3) << std::endl;

        recipe_common::print_collection(nums, "nums: ");
        std::sort(nums.begin(), nums.end(), std::greater<>());
        recipe_common::print_collection(nums, "nums: ");
        std::cout << std::endl;
    }

    // function adapter
    {
        // deprecated
        int count = std::count_if(nums.begin(), nums.end(),
                std::bind1st(IntCompare(), 5));
        std::cout << count << std::endl;

        count = std::count_if(nums.begin(), nums.end(),
                std::bind2nd(IntCompare(), 5));
        std::cout << count << std::endl;

        count = std::count_if(nums.begin(), nums.end(), std::not1(IsOdd()));
        std::cout << count << std::endl;

        nums.assign(ar.begin(), ar.end());
        std::sort(nums.begin(), nums.end(), std::not2(IntCompare()));
        recipe_common::print_collection(nums, "nums: ");

        // preferred
        count = std::count_if(nums.begin(), nums.end(),
                std::bind(IntCompare(), 5, std::placeholders::_1));
        std::cout << count << std::endl;

        count = std::count_if(nums.begin(), nums.end(),
                std::bind(IntCompare(), std::placeholders::_1, 5));
        std::cout << count << std::endl;

        count = std::count_if(nums.begin(), nums.end(), std::not_fn(IsOdd()));
        std::cout << count << std::endl;

        nums.assign(ar.begin(), ar.end());
        std::sort(nums.begin(), nums.end(), std::not_fn(IntCompare()));
        recipe_common::print_collection(nums, "nums: ");
        std::cout << std::endl;
    }

    // functional compostion
    {
        // deprecated
        nums.assign(ar.begin(), ar.end());
        int count = std::count_if(nums.begin(), nums.end(),
                std::not1(std::bind2nd(std::modulus<int>(), 3)));
        std::cout << count << std::endl;

        // preferred
        count = std::count_if(nums.begin(), nums.end(),
                std::not_fn(std::bind(std::modulus<int>(), std::placeholders::_1, 3)));
        std::cout << count << std::endl;
        std::cout << std::endl;
    }

    // function adapter for common function
    {
        // deprecated
        int count = std::count_if(nums.begin(), nums.end(),
                std::not1(std::ptr_fun(is_odd)));
        std::cout << count << std::endl;

        // preferred
        count = std::count_if(nums.begin(), nums.end(),
                std::not_fn(is_odd));
        std::cout << count << std::endl;
        std::cout << std::endl;
    }

    // function adapter for member function
    {
        std::array<Student, 4> ar = {
            Student("Alice", 85),
            Student("Bob", 92),
            Student("Charlie", 78),
            Student("David", 96)
        };
        std::vector<Student> students;
        students.assign(ar.begin(), ar.end());

        // deprecated
        std::for_each(students.begin(), students.end(),
                std::bind2nd(std::mem_fun_ref(&Student::AddScore), 10));
        std::for_each(students.begin(), students.end(),
                std::mem_fun_ref(&Student::Display));
        std::cout << std::endl;

        // preferred
        students.assign(ar.begin(), ar.end());
        std::for_each(students.begin(), students.end(),
                std::bind(&Student::AddScore, std::placeholders::_1, 10));
        std::for_each(students.begin(), students.end(),
                std::mem_fn(&Student::Display));
        std::cout << std::endl;
    }

    // lambda
    {
        auto s = [] (int a, int b) {
            return a + b;
        }(2, 3);
        std::cout << s << std::endl;

        int count = std::count_if(nums.begin(), nums.end(),
                [] (int x) {
                    return !is_odd(x);
                });
        std::cout << count << std::endl;

        std::sort(nums.begin(), nums.end(), [] (int a, int b) {
            return a > b;
        });
        recipe_common::print_collection(nums, "nums: ");
    }
}

}
