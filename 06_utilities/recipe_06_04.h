#pragma once

#include "recipe_06_common.h"

#include <chrono>
#include <functional>

namespace recipe_06_04
{

void func(int const count = 100000000)
{
    for (int i = 0; i < count; ++i);
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

template <typename T>
void print_clock(std::string const &clock_name)
{
    std::cout << clock_name << ": ";
    std::cout << "precision = " <<
            (1000000.0 * double(T::period::num)) / (T::period::den) << ", ";
    std::cout << "is_steady = " << std::boolalpha << T::is_steady << std::endl;
}

void execute()
{
    // measure the execution of a function
    {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto diff = std::chrono::high_resolution_clock::now() - start;

        std::cout << std::chrono::duration<double, std::milli>(diff).count() <<
                "ms" << std::endl;
        std::cout << std::fixed <<
                std::chrono::duration<double, std::nano>(diff).count() <<
                "ns" << std::endl;
        std::cout << std::defaultfloat << std::endl;
    }

    // measure the execution of a function with a reusable component
    {
        auto t = perf_timer<>::duration(func, 100000000);

        std::cout << std::chrono::duration<double, std::milli>(t).count() <<
                "ms" << std::endl;
        std::cout << std::fixed <<
                std::chrono::duration<double, std::nano>(t).count() <<
                "ns" << std::endl;
        std::cout << std::defaultfloat << std::endl;
    }

    // print the precision of three clocks
    {
        print_clock<std::chrono::system_clock>("system_clock         ");
        print_clock<std::chrono::high_resolution_clock>("high_resolution_clock");
        print_clock<std::chrono::steady_clock>("steady_clock         ");
        std::cout << std::endl;
    }

    // return duration value rather than number of ticks
    {
        auto t1 = perf_timer<std::chrono::nanoseconds>::duration(func, 100000000);
        std::cout << t1 << std::endl;
        auto t2 = perf_timer<std::chrono::microseconds>::duration(func, 100000000);
        std::cout << t2 << std::endl;
        auto t3 = perf_timer<std::chrono::milliseconds>::duration(func, 100000000);
        std::cout << t3 << std::endl;

        auto total = t1 + t2 + t3;
        std::cout << std::chrono::duration<double, std::micro>(total).count() <<
                "us" << std::endl;
    }
}

}
