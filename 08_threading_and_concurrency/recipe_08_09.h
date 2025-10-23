#pragma once

#include "recipe_08_common.h"

#include <algorithm>
#include <numeric>
#include <thread>

namespace recipe_08_09
{

unsigned int get_num_of_threads()
{
    return std::thread::hardware_concurrency();
}

template <typename Iter, typename F>
void parallel_map(Iter begin, Iter end, F f)
{
    auto size = std::distance(begin, end);

    if (size <= 10000) {
        std::transform(begin, end, begin, std::forward<F>(f));
    } else {
        auto num_of_threads = get_num_of_threads();
        auto part = size / num_of_threads;
        auto last = begin;

        std::vector<std::thread> threads;
        for (unsigned i = 0; i < num_of_threads; ++i) {
            if (i == num_of_threads - 1) {
                last = end;
            } else {
                std::advance(last, part);
            }

            threads.emplace_back([=, &f] () {
                std::transform(begin, last, begin, std::forward<F>(f));
            });

            begin = last;
        }

        for (auto &t : threads) {
            t.join();
        }
    }
}

template <typename Iter, typename R, typename F>
auto parallel_fold(Iter begin, Iter end, R init, F op)
{
    auto size = std::distance(begin, end);

    if (size <= 10000) {
        return std::accumulate(begin, end, init, std::forward<F>(op));
    } else {
        auto num_of_threads = get_num_of_threads();
        auto part = size / num_of_threads;
        auto last = begin;

        std::vector<std::thread> threads;
        std::vector<R> values(num_of_threads);
        for (unsigned i = 0; i < num_of_threads; ++i) {
            if (i == num_of_threads - 1) {
                last = end;
            } else {
                std::advance(last, part);
            }

            threads.emplace_back([=, &op] (R &result) {
                result = std::accumulate(begin, last, R {}, std::forward<F>(op));
            }, std::ref(values[i]));

            begin = last;
        }

        for (auto &t : threads) {
            t.join();
        }

        return std::accumulate(std::begin(values), std::end(values),
                init, std::forward<F>(op));
    }
}

void execute()
{
    // run both the sequential and parallel versions of map and fold
    // on vectors of different sizes and print the execution time
    {
        std::vector<int> sizes {
            10000, 100000, 500000,
            1000000, 2000000, 5000000,
            10000000, 25000000, 50000000
        };

        std::cout << std::fixed << std::setprecision(0) <<
                std::right << std::setw(10) << std::setfill(' ') << "size" <<
                std::right << std::setw(10) << "s map" <<
                std::right << std::setw(10) << "p map" <<
                std::right << std::setw(10) << "s fold" <<
                std::right << std::setw(10) << "p fold" << std::endl;

        for (auto const size : sizes) {
            std::vector<int> v(size);
            std::iota(std::begin(v), std::end(v), 1);

            // sequential version
            auto v1 = v;
            auto s1 = 0LL;
            auto tsm = recipe_common::perf_timer<>::duration([&] () {
                std::transform(std::begin(v1), std::end(v1), std::begin(v1),
                        [] (int const i) {
                            return i + i;
                        });
            });
            auto tsf = recipe_common::perf_timer<>::duration([&] () {
                s1 = std::accumulate(std::begin(v1), std::end(v1), 0LL, std::plus<>());
            });

            // parallel version
            auto v2 = v;
            auto s2 = 0LL;
            auto tpm = recipe_common::perf_timer<>::duration([&] () {
                parallel_map(std::begin(v2), std::end(v2),
                        [] (int const i) {
                            return i + i;
                        });
            });
            auto tpf = recipe_common::perf_timer<>::duration([&] () {
                s2 = parallel_fold(std::begin(v2), std::end(v2), 0LL, std::plus<>());
            });

            assert(v1 == v2);
            assert(s1 == s2);

            std::cout << std::fixed << std::setprecision(0) <<
                    std::right << std::setw(10) << std::setfill(' ') << size <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tsm).count() <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tpm).count() <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tsf).count() <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tpf).count() << std::endl;
        }
    }
}

}
