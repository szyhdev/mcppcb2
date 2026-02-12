#pragma once

#include "recipe_02_common.h"

#include <random>

namespace recipe_02_04
{

double monte_carlo_simulation()
{
    std::array<int, std::mt19937::state_size> seed_data {};
    std::random_device rd {};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    auto eng = std::mt19937 { seq };
    auto dist = std::uniform_real_distribution<> { 0, 1 };

    auto const simulations = 5;
    auto const samples = 1000000;

    double total = 0.0;
    for (auto j = 0; j < simulations; ++j) {
        auto hit = 0;
        auto x = 0.0;
        auto y = 0.0;

        for (auto i = 0; i < samples; ++i) {
            x = dist(eng);
            y = dist(eng);

            if (y <= std::sqrt(1 - std::pow(x, 2))) {
                hit += 1;
            }
        }

        total += 4.0 * hit / samples;
    }

    return total / simulations;
}

void execute()
{
    std::cout << monte_carlo_simulation() << std::endl;
}

}
