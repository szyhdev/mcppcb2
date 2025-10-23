#pragma once

#include <iostream>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <functional>
#include <queue>
#include <sstream>

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

template <typename Time = std::chrono::microseconds,
        typename Clock = std::chrono::high_resolution_clock>
struct perf_timer
{
    template <typename F, typename ...Args>
    static Time duration(F &&f, Args ...args) {
        auto start = Clock::now();
        std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
        auto end = Clock::now();
        return std::chrono::duration_cast<Time>(end - start);
    }
};

}
