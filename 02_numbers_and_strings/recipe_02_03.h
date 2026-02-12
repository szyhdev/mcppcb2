#pragma once

#include "recipe_02_common.h"

#include <random>

namespace recipe_02_03
{

void generate_and_print(std::function<int(void)> gen,
        int const iterations = 10000)
{
    // generate random numbers
    auto data = std::map<int, int> {};
    for (auto n = 0; n < iterations; ++n) {
        ++data[gen()];
    }

    // find the element with most repetitions
    auto max = std::max_element(std::begin(data), std::end(data),
            [] (auto kvp1, auto kvp2) {
                return kvp1.second < kvp2.second;
            });

    // print the bars
    for (auto i = max->second / 200; i > 0; --i) {
        std::cout << " ";
        for (auto kvp : data) {
            std::cout << (kvp.second / 200 >= i ? " \u2588 " : "   "); // \u2588 = █
        }
        std::cout << std::endl;
    }

    // print the numbers
    for (auto kvp : data) {
        std::cout << std::fixed << std::setprecision(1) << std::setw(3) << kvp.first;
    }
    std::cout << std::defaultfloat << std::setprecision(6) << std::setw(0);
    std::cout << std::endl;
}

void execute()
{
    // simplest way to generate random numbers
    {
        auto mtgen = std::mt19937 {};

        for (auto i = 0; i < 5; ++i) {
            std::cout << mtgen() << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }

    // use number of current ticks to seed the engine
    {
        auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        auto mtgen = std::mt19937 { static_cast<unsigned int>(seed) };

        for (auto i = 0; i < 5; ++i) {
            std::cout << mtgen() << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }

    // use actual random number to seed the engine
    {
        std::random_device rd {};
        auto mtgen = std::mt19937 { rd() };

        for (auto i = 0; i < 5; ++i) {
            std::cout << mtgen() << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }

    // a uniform distribution in the range [1, 6]
    {
        std::random_device rd {};
        auto mtgen = std::mt19937 { rd() };
        auto ud = std::uniform_int_distribution<> { 1, 6 };
        generate_and_print(
                [&mtgen, &ud] () {
                    return ud(mtgen);
                });
        std::cout << std::endl;
    }

    // a normal distribution with a mean of 5 and a standard deviation of 2
    {
        std::random_device rd {};
        auto mtgen = std::mt19937 { rd() };
        auto nd = std::normal_distribution<> { 5, 2 };
        generate_and_print(
                [&mtgen, &nd] () {
                    return static_cast<int>(std::round(nd(mtgen)));
                });
    }
}

}
