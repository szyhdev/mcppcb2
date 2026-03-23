#pragma once

#include "recipe_03_common.h"

namespace recipe_03_08
{

unsigned int compose_way(std::vector<int> const &v, int loop)
{
    unsigned int total = 0;
    for (int i = 0; i < loop; ++i) {
        auto s = recipe_03_common::compose([] (std::vector<int> const &v) {
            return recipe_03_common::foldl(std::plus<>(), v, 0);
        }, [] (std::vector<int> const & v) {
            return recipe_03_common::mapf([] (int const i) {
                return i * i;
            }, v);
        }, [] (std::vector<int> const &v) {
            return recipe_03_common::mapf([] (int const i) {
                return std::abs(i);
            }, v);
        })(v);
        total += s;
    }

    return total;
}

unsigned int pipeline_way(std::vector<int> const &v, int loop)
{
    unsigned int total = 0;
    for (int i = 0; i < loop; ++i) {
        auto s = recipe_03_common::foldl(std::plus<>(),
            recipe_03_common::mapf([] (int const i) {
                return i * i;
            }, recipe_03_common::mapf([] (int const i) {
                return std::abs(i);
            }, v)),
        0);
        total += s;
    }

    return total;
}

template <typename F, typename G>
auto operator *(F &&f, G &&g)
{
    return recipe_03_common::compose(std::forward<F>(f), std::forward<G>(g));
}

template <typename F, typename ...R>
auto operator *(F &&f, R &&...rest)
{
    return operator *(std::forward<F>(f), rest...);
}

void execute()
{
    auto vnums = std::vector<int> { 0, 2, -3, 5, -1, 6, 8, -4, 9 };

    // compose two functions
    {
        auto n = recipe_03_common::compose([] (int const n) {
                    return std::to_string(n);
                }, [] (int const n) {
                    return n * n;
                })(-3);
        std::cout << n << std::endl;
        std::cout << std::endl;
    }

    // compose a variable number of functions
    {
        auto n = recipe_03_common::compose([] (int const n) {
                    return std::to_string(n);
                }, [] (int const n) {
                    return n * n;
                }, [] (int const n) {
                    return n + n;
                }, [] (int const n) {
                    return std::abs(n);
                })(-3);
        std::cout << n << std::endl;
        std::cout << std::endl;
    }

    // rewrite last example in previous recipe
    {
        auto s = recipe_03_common::compose([] (std::vector<int> const &v) {
                return recipe_03_common::foldl(std::plus<>(), v, 0);
            }, [] (std::vector<int> const & v) {
                return recipe_03_common::mapf([] (int const i) {
                    return i * i;
                }, v);
            }, [] (std::vector<int> const &v) {
                return recipe_03_common::mapf([] (int const i) {
                    return std::abs(i);
                }, v);
            })(vnums);
        std::cout << s << std::endl;
        std::cout << std::endl;
    }

    // performance comparison
    {
        auto t1 = recipe_common::perf_timer<std::chrono::milliseconds>::duration(
                compose_way, vnums, 1000000);
        std::cout << t1 << std::endl;
        auto t2 = recipe_common::perf_timer<std::chrono::milliseconds>::duration(
                pipeline_way, vnums, 1000000);
        std::cout << t2 << std::endl;
        std::cout << std::endl;
    }

    // compose functions by overloading operator *
    {
        auto n = ([] (int const n) {
                    return std::to_string(n);
                } * [] (int const n) {
                    return n * n;
                } * [] (int const n) {
                    return n + n;
                } * [] (int const n) {
                    return std::abs(n);
                })(-3);
        std::cout << n << std::endl;

        auto s = ([] (std::vector<int> const &v) {
                    return recipe_03_common::foldl(std::plus<>(), v, 0);
                } * [] (std::vector<int> const &v) {
                    return recipe_03_common::mapf([] (int const i) {
                        return i * i;
                    }, v);
                } * [] (std::vector<int> const &v) {
                    return recipe_03_common::mapf([] (int const i) {
                        return std::abs(i);
                    }, v);
                })(vnums);
        std::cout << s << std::endl;
    }
}

}
