#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cmath>
#include <execution>
#include <functional>
#include <future>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <thread>
#include <vector>

namespace recipe_common
{

std::ostream &operator <<(std::ostream &os, const std::pair<std::string, int> &e)
{
    os << "(" << e.first << ", " << e.second << ")";
    return os;
}

template <typename T>
void print_collection(const T &coll, const std::string &prefix = "")
{
    std::cout << prefix;
    for (auto &e : coll) {
        std::cout << e << " ";
    }
    std::cout << std::endl;
}

template <typename T>
void print_queue(const std::queue<T> &q, const std::string &prefix = "")
{
    std::cout << prefix;
    std::queue<int> temp(q);
    while (!temp.empty()) {
        std::cout << temp.front() << " ";
        temp.pop();
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
