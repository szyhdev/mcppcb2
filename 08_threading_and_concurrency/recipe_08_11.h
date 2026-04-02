#pragma once

#include "recipe_08_09.h"
#include "recipe_08_10.h"

namespace recipe_08_11
{

void generate_data(std::vector<int> &data, int size)
{
    std::array<int, std::mt19937::state_size> seed_data {};
    std::random_device rd {};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    auto eng = std::mt19937 { seq };
    auto dist = std::uniform_int_distribution<> { 1, size };

    data.resize(size);
    for (auto i = 0; i < size; ++i) {
        data[i] = dist(eng);
    }
}

template <typename Iter, typename F>
void parallel_map(Iter begin, Iter end, F f)
{
    std::transform(std::execution::par, begin, end,
            begin, std::forward<F>(f));
}

template <typename Iter, typename R, typename F>
auto parallel_reduce(Iter begin, Iter end, R init, F op)
{
    return std::reduce(std::execution::par, begin, end,
            init, std::forward<F>(op));
}

void execute()
{
    std::vector<int> sizes {
        10000, 100000, 500000,
        1000000, 2000000, 5000000,
        10000000, 25000000, 50000000
    };

    // run std::sort and its overloaded parallel execution
    // on vectors of different sizes and print the execution time
    {
        std::cout << std::fixed << std::setprecision(0) <<
                std::right << std::setw(10) << std::setfill(' ') << "size" <<
                std::right << std::setw(10) << "s sort" <<
                std::right << std::setw(10) << "p sort" << std::endl;

        for (auto const size : sizes) {
            std::vector<int> v;
            generate_data(v, size);

            // sequential version
            auto v1 = v;
            auto tss = recipe_common::perf_timer<>::duration([&] () {
                std::sort(std::execution::seq, std::begin(v1), std::end(v1));
            });

            // parallel version
            auto v2 = v;
            auto tps = recipe_common::perf_timer<>::duration([&] () {
                std::sort(std::execution::par, std::begin(v2), std::end(v2));
            });

            std::cout << std::fixed << std::setprecision(0) <<
                    std::right << std::setw(10) << std::setfill(' ') << v.size() <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tss).count() <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tps).count() << std::endl;
        }
        std::cout << std::endl;
    }

    // run std::accumulate and its parallel version of std::reduce
    // on vectors of different sizes and print the execution time
    {
        std::cout << std::fixed << std::setprecision(0) <<
                std::right << std::setw(10) << std::setfill(' ') << "size" <<
                std::right << std::setw(12) << "accumulate" <<
                std::right << std::setw(12) << "reduce" << std::endl;

        for (auto const size : sizes) {
            std::vector<int> v(size);
            std::iota(std::begin(v), std::end(v), 1);

            // accumulate
            auto v1 = v;
            auto s1 = 0LL;
            auto ts = recipe_common::perf_timer<>::duration([&] () {
                s1 = std::accumulate(std::begin(v1), std::end(v1), 0LL, std::plus<>());
            });

            // reduce
            auto v2 = v;
            auto s2 = 0LL;
            auto tp = recipe_common::perf_timer<>::duration([&] () {
                s2 = std::accumulate(std::begin(v2), std::end(v2), 0LL, std::plus<>());
            });

            assert(v1 == v2);
            assert(s1 == s2);

            std::cout << std::fixed << std::setprecision(0) <<
                    std::right << std::setw(10) << std::setfill(' ') << size <<
                    std::right << std::setw(12) <<
                    std::chrono::duration<double, std::micro>(ts).count() <<
                    std::right << std::setw(12) <<
                    std::chrono::duration<double, std::micro>(tp).count() << std::endl;
        }
        std::cout << std::endl;
    }

    // run sequential version, parallel thread version, parallel task
    // version, parallel algorithm version, sequential transform_reduce
    // version and parallel transform_reduce version of map and fold
    // on vectors of different sizes and print the execution time
    {
        std::cout << std::fixed << std::setprecision(0) <<
                std::right << std::setw(10) << std::setfill(' ') << "size" <<
                std::right << std::setw(10) << "s map" <<
                std::right << std::setw(10) << "p map" <<
                std::right << std::setw(10) << "pt map" <<
                std::right << std::setw(10) << "pa map" <<
                std::right << std::setw(10) << "s fold" <<
                std::right << std::setw(10) << "p fold" <<
                std::right << std::setw(10) << "pt fold" <<
                std::right << std::setw(10) << "pa fold" <<
                std::right << std::setw(12) << "s map_fold" <<
                std::right << std::setw(12) << "p map_fold" << std::endl;

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
                s1 = std::accumulate(std::begin(v1), std::end(v1),
                        0LL, std::plus<>());
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
                recipe_08_10::v1::parallel_map(std::begin(v3), std::end(v3),
                        [] (int const i) {
                            return i + i;
                        });
            });
            auto tptf = recipe_common::perf_timer<>::duration([&] () {
                s3 = recipe_08_10::v1::parallel_fold(std::begin(v3), std::end(v3),
                        0LL, std::plus<>());
            });

            // parallel algorithm version
            auto v4 = v;
            auto s4 = 0LL;
            auto tpam = recipe_common::perf_timer<>::duration([&] {
                parallel_map(std::begin(v4), std::end(v4),
                        [] (int const i) {
                            return i + i;
                        });
            });
            auto tpaf = recipe_common::perf_timer<>::duration([&] {
                s4 = parallel_reduce(std::begin(v4), std::end(v4),
                        0LL, std::plus<>());
            });

            // sequential transform_reduce version
            auto v5 = v;
            auto s5 = 0LL;
            auto tsmf = recipe_common::perf_timer<>::duration([&] {
                s5 = std::transform_reduce(std::execution::seq,
                        std::begin(v5), std::end(v5), 0LL, std::plus<>(),
                        [] (int const i) {
                            return i + i;
                        });
            });

            // parallel transform_reduce version
            auto v6 = v;
            auto s6 = 0LL;
            auto tpmf = recipe_common::perf_timer<>::duration([&] {
                s6 = std::transform_reduce(std::execution::par_unseq,
                        std::begin(v6), std::end(v6), 0LL, std::plus<>(),
                        [] (int const i) {
                            return i + i;
                        });
            });

            assert(v1 == v2);
            assert(v2 == v3);
            assert(v3 == v4);

            assert(s1 == s2);
            assert(s2 == s3);
            assert(s3 == s4);
            assert(s4 == s5);
            assert(s5 == s6);

            std::cout << std::fixed << std::setprecision(0) <<
                    std::right << std::setw(10) << std::setfill(' ') << size <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tsm).count() <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tpm).count() <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tptm).count() <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tpam).count() <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tsf).count() <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tpf).count() <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tptf).count() <<
                    std::right << std::setw(10) <<
                    std::chrono::duration<double, std::micro>(tpaf).count() <<
                    std::right << std::setw(12) <<
                    std::chrono::duration<double, std::micro>(tsmf).count() <<
                    std::right << std::setw(12) <<
                    std::chrono::duration<double, std::micro>(tpmf).count() << std::endl;
        }
    }
}

}
