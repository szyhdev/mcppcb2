#pragma once

#include "recipe_12_common.h"

// #define USE_RANGE_V3

#ifdef USE_RANGE_V3

#include "range/v3/algorithm/sort.hpp"
#include "range/v3/algorithm/copy.hpp"
#include "range/v3/algorithm/reverse.hpp"
#include "range/v3/algorithm/count_if.hpp"
#include "range/v3/algorithm/max.hpp"
#include "range/v3/iterator/stream_iterators.hpp"
#include "range/v3/view.hpp"

namespace rv = ranges::views;
namespace rg = ranges;

#define OSTREAM_ITERATOR ranges::ostream_iterator

#else

namespace rv = std::ranges::views;
namespace rg = std::ranges;

#define OSTREAM_ITERATOR std::ostream_iterator

#endif

namespace recipe_12_05
{

bool is_prime(int const number)
{
    if (number != 2) {
        if ((number < 2) || (number % 2 == 0)) {
            return false;
        }

        auto root = std::sqrt(number);
        for (int i = 3; i <= root; i += 2) {
            if (number % i == 0) {
                return false;
            }
        }
    }

    return true;
}

struct stop_at_sentinel
{
    char end = '\0';

    friend bool operator ==(const char *itr, stop_at_sentinel sas) {
        return *itr == sas.end;
    }
};

struct person
{
    std::string name;
    int age;
};

void stl_func(std::vector<int> const &src_vec, std::vector<int> &dst_vec, int loop)
{
    dst_vec.reserve(dst_vec.size() + loop * 3);

    for (int i = 0; i < loop; ++i) {
        std::vector<int> evens;
        std::copy_if(src_vec.begin(), src_vec.end(), std::back_inserter(evens),
                [] (int n) {
                    return n % 2 == 0;
                });

        std::vector<int> squared;
        std::transform(evens.begin(), evens.end(), std::back_inserter(squared),
                [] (int n) {
                    return n * n;
                });

        for (int i = 0; i < 3 && i < squared.size(); ++i) {
            dst_vec.push_back(squared[i]);
        }
    }
}

void range_func(std::vector<int> const &src_vec, std::vector<int> &dst_vec, int loop)
{
    dst_vec.reserve(dst_vec.size() + loop * 3);

    for (int i = 0; i < loop; ++i) {
        auto result = src_vec | rv::filter([] (int n) { return n % 2 == 0; }) |
                rv::transform([] (int n) { return n * n; }) | rv::take(3);
        for (int n : result) {
            dst_vec.push_back(n);
        }
    }
}

void execute()
{
    std::vector<int> nums { 0, 1, 1, 2, 3, 5, 8, 13 };

    // various components of C++20 ranges library
    {
        auto v = nums | rv::filter(is_prime) | rv::take(3) | rv::reverse;
        for (auto i : v) {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        int num = 3;
        auto itr1 = std::ranges::find(nums, num);
        if (itr1 != nums.end()) {
            std::cout << num << " found" << std::endl;
        }

        char ch1 = 'R';
        char str[] = "Hello, C++20 Ranges";
        stop_at_sentinel sas {};
        auto itr2 = std::ranges::find(str, sas, ch1);
        if (*itr2 != sas.end) {
            std::cout << ch1 << " found" << std::endl;
        }

        std::vector<int> primes { 2, 3, 5 };
        auto match = std::ranges::search(nums, primes);
        if (!match.empty()) {
            std::ranges::copy(match, OSTREAM_ITERATOR<int>(std::cout, " "));
            std::cout << std::endl;
        }

        auto itr3 = std::ranges::find(std::vector<int> { 2, 3, 5 }, 3);
        // std::cout << *itr3 << std::endl;  // compile error

        char ch2 = 'r';
        auto itr4 = std::ranges::find(str, ch2, tolower);
        if (itr4 != std::end(str)) {
            std::cout << ch2 << " found" << std::endl;
        }

        std::vector<person> people = {
            {"Alice", 30}, {"Bob", 25}, {"Charlie", 35}
        };

        auto itr5 = std::ranges::find(people, 25, &person::age);
        if (itr5 != people.end()) {
            std::cout << itr5->name << " found" << std::endl;
        }

        auto source = std::views::iota(0);
        auto pipeline = source
                | std::views::filter([] (int i) { return i % 2 != 0; })
                | std::views::transform([] (int i) { return i * i; })
                | std::views::take(5);
        for (int n : pipeline) {
            std::cout << n << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }

    // use views
    {
        // generate a sequence of consecutive integers
        for (auto i : rv::iota(1, 10)) {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        // filter the elements of a range
        for (auto i : rv::iota(1, 20) | rv::filter(is_prime)) {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        for (auto i : nums | rv::filter(is_prime)) {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        // transform the elements of a range
        for (auto i : rv::iota(1, 20) | rv::filter(is_prime) |
                rv::transform([] (int const n) {
                    return n + 1;
                })) {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        // retain only the first N elements of a view
        for (auto i : rv::iota(1, 20) | rv::filter(is_prime) | rv::take(5)) {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        // iterate a range in reverse order
        for (auto i : rv::iota(1, 20) | rv::reverse |
                rv::filter(is_prime) | rv::take(5)) {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        for (auto i : rv::iota(1, 20) | rv::reverse |
                rv::filter(is_prime) | rv::take(5) | rv::reverse) {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        // skip the first N elements of a range
        for (auto i : rv::iota(1, 20) | rv::filter(is_prime) |
                rv::drop(3) | rv::reverse | rv::drop(3) | rv::reverse) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }

    // use algorithms
    {
        std::vector<int> v { 5, 2, 7, 1, 4, 2, 9, 5 };

        // determine the maximum element of a range
        std::cout << rg::max(v) << std::endl;

        // sort a range
        rg::sort(v);

        // copy a range
        rg::copy(v, OSTREAM_ITERATOR<int>(std::cout, " "));
        std::cout << std::endl;

        // reverse the elements of a range
        rg::reverse(v);
        rg::copy(v, OSTREAM_ITERATOR<int>(std::cout, " "));
        std::cout << std::endl;

        // count the elements of a range that verify a predicate
        std::cout << rg::count_if(v, is_prime) << std::endl;
        std::cout << std::endl;
    }

    // performance comparison
    {
        std::vector<int> nums(100);
        std::iota(nums.begin(), nums.end(), 1);

        std::vector<int> res1;
        auto t1 = recipe_common::perf_timer<std::chrono::milliseconds>::duration(
                stl_func, nums, res1, 100000);
        std::vector<int> res2;
        auto t2 = recipe_common::perf_timer<std::chrono::milliseconds>::duration(
                range_func, nums, res2, 100000);
        assert(res1 == res2);
        std::cout << t1.count() << "ms" << std::endl;
        std::cout << t2.count() << "ms" << std::endl;
    }
}

}
