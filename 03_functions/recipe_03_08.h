#pragma once

#include "recipe_03_common.h"
#include "funclib.h"

#include <string>

namespace recipe_03_08
{

template <typename F, typename G>
auto operator *(F &&f, G &&g)
{
    return funclib::compose(std::forward<F>(f), std::forward<G>(g));
}

template <typename F, typename ...R>
auto operator *(F &&f, R &&...r)
{
    return operator *(std::forward<F>(f), r...);
}

void execute()
{
    auto vnums = std::vector<int> { 0, 2, -3, 5, -1, 6, 8, -4, 9 };

    // compose two functions
    {
        auto n = funclib::compose(
                [] (int const n) {
                    return std::to_string(n);
                },
                [] (int const n) {
                    return n * n;
                })(-3);
        std::cout << n << std::endl;
        std::cout << std::endl;
    }

    // compose a variable number of functions
    {
        auto n = funclib::compose(
                [] (int const n) {
                    return std::to_string(n);
                },
                [] (int const n) {
                    return n * n;
                },
                [] (int const n) {
                    return n + n;
                },
                [] (int const n) {
                    return std::abs(n);
                })(-3);
        std::cout << n << std::endl;
        std::cout << std::endl;
    }

    // rewrite last example in previous recipe
    {
        auto s = funclib::compose(
                [] (std::vector<int> const &v) {
                    return funclib::foldl(std::plus<>(), v, 0);
                },
                [] (std::vector<int> const & v) {
                    return funclib::mapf([] (int const i) {
                        return i + i;
                    }, v);
                },
                [] (std::vector<int> const &v) {
                    return funclib::mapf([] (int const i) {
                        return std::abs(i);
                    }, v);
                })(vnums);
        std::cout << s << std::endl;
        std::cout << std::endl;
    }

    // compose functions by overloading operator *
    {
        auto n = (
                [] (int const n) {
                    return std::to_string(n);
                } *
                [] (int const n) {
                    return n * n;
                } *
                [] (int const n) {
                    return n + n;
                } *
                [] (int const n) {
                    return std::abs(n);
                })(-3);
        std::cout << n << std::endl;

        auto c =
                [] (std::vector<int> const &v) {
                    return funclib::foldl(std::plus<>(), v, 0);
                } *
                [] (std::vector<int> const &v) {
                    return funclib::mapf([] (int const i) {
                        return i + i;
                    }, v);
                } *
                [] (std::vector<int> const &v) {
                    return funclib::mapf([] (int const i) {
                        return std::abs(i);
                    }, v);
                };
        auto s = c(vnums);
        std::cout << s << std::endl;
    }
}

}
