#pragma once

#include "recipe_08_09.h"

namespace recipe_08_10
{

namespace v1
{

template <typename Iter, typename F>
void parallel_map(Iter begin, Iter end, F f)
{
    auto size = std::distance(begin, end);

    if (size <= 10000) {
        std::transform(begin, end, begin, std::forward<F>(f));
    } else {
        auto num_of_tasks = get_num_of_threads();
        auto part = size / num_of_tasks;
        auto last = begin;

        std::vector<std::future<void>> tasks;
        for (unsigned i = 0; i < num_of_tasks; ++i) {
            if (i == num_of_tasks - 1) {
                last = end;
            } else {
                std::advance(last, part);
            }

            tasks.emplace_back(std::async(std::launch::async, [=, &f] () {
                std::transform(begin, last, begin, std::forward<F>(f));
            }));

            begin = last;
        }

        for (auto &f : tasks) {
            f.wait();
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
        auto num_of_tasks = get_num_of_threads();
        auto part = size / num_of_tasks;
        auto last = begin;

        std::vector<std::future<R>> tasks;
        for (unsigned i = 0; i < num_of_tasks; ++i) {
            if (i == num_of_tasks - 1) {
                last = end;
            } else {
                std::advance(last, part);
            }

            tasks.emplace_back(std::async(std::launch::async, [=, &op] () {
                return std::accumulate(begin, last, R {}, std::forward<F>(op));
            }));

            begin = last;
        }

        std::vector<R> values;
        for (auto &f : tasks) {
            values.push_back(f.get());
        }

        return std::accumulate(std::begin(values), std::end(values),
                init, std::forward<F>(op));
    }
}

}

namespace v2
{

template <typename Iter, typename F>
void parallel_map(Iter begin, Iter end, F f)
{
    auto size = std::distance(begin, end);

    if (size <= 10000) {
        std::transform(begin, end, begin, std::forward<F>(f));
    } else {
        auto middle = begin;
        std::advance(middle, size / 2);

        auto result = std::async(std::launch::deferred,
                v2::parallel_map<Iter, F>, begin, middle, std::forward<F>(f));
        v2::parallel_map(middle, end, std::forward<F>(f));
        result.wait();
    }
}

template <typename Iter, typename R, typename F>
auto parallel_fold(Iter begin, Iter end, R init, F op)
{
    auto size = std::distance(begin, end);

    if (size <= 10000) {
        return std::accumulate(begin, end, init, std::forward<F>(op));
    } else {
        auto middle = begin;
        std::advance(middle, size / 2);

        auto result1 = std::async(std::launch::async,
                v2::parallel_fold<Iter, R, F>, begin, middle, R {}, std::forward<F>(op));
        auto result2 = v2::parallel_fold(middle, end, init, std::forward<F>(op));
        return result1.get() + result2;
    }
}

}

void execute()
{
    // run sequential version, parallel task version and parallel recursion version
    // of map and fold on vectors of different sizes and print the execution time
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
                std::right << std::setw(10) << "pt map" <<
                std::right << std::setw(10) << "pr map" <<
                std::right << std::setw(10) << "s fold" <<
                std::right << std::setw(10) << "p fold" <<
                std::right << std::setw(10) << "pt fold" <<
                std::right << std::setw(10) << "pr fold" << std::endl;

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

            // parallel thread version
            auto v2 = v;
            auto s2 = 0LL;
            auto tpm = recipe_common::perf_timer<>::duration([&] () {
                recipe_08_09::parallel_map(std::begin(v2), std::end(v2),
                        [] (int const i) {
                            return i + i;
                        });
            });
            auto tpf = recipe_common::perf_timer<>::duration([&] () {
                s2 = recipe_08_09::parallel_fold(std::begin(v2), std::end(v2),
                        0LL, std::plus<>());
            });


            // parallel task version
            auto v3 = v;
            auto s3 = 0LL;
            auto tptm = recipe_common::perf_timer<>::duration([&] () {
                v1::parallel_map(std::begin(v3), std::end(v3),
                        [] (int const i) {
                            return i + i;
                        });
            });
            auto tptf = recipe_common::perf_timer<>::duration([&] () {
                s3 = v1::parallel_fold(std::begin(v3), std::end(v3), 0LL, std::plus<>());
            });

            // parallel recursion version
            auto v4 = v;
            auto s4 = 0LL;
            auto tprm = recipe_common::perf_timer<>::duration([&] () {
                v2::parallel_map(std::begin(v4), std::end(v4),
                        [] (int const i) {
                            return i + i;
                        });
            });
            auto tprf = recipe_common::perf_timer<>::duration([&] () {
                s4 = v2::parallel_fold(std::begin(v4), std::end(v4), 0LL, std::plus<>());
            });

            assert(v1 == v2);
            assert(v2 == v3);
            assert(v3 == v4);

            assert(s1 == s2);
            assert(s2 == s3);
            assert(s3 == s4);

            std::cout << std::fixed << std::setprecision(0) <<
                    std::right << std::setw(10) << std::setfill(' ') << size <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tsm).count() <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tpm).count() <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tptm).count() <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tprm).count() <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tsf).count() <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tpf).count() <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tptf).count() <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tprf).count() << std::endl;
        }
    }
}

}
