#pragma once

#include "recipe_03_common.h"
#include "funclib.h"

#include <array>
#include <cmath>
#include <list>
#include <map>
#include <queue>
#include <vector>

namespace recipe_03_07
{

using namespace std::string_literals;

template <class T = double>
struct fround
{
    typename std::enable_if_t<std::is_floating_point_v<T>, T>
    operator ()(const T &value) const {
        return std::round(value);
    }
};

void execute()
{
    auto vnums = std::vector<int> { 0, 2, -3, 5, -1, 6, 8, -4, 9 };
    auto words = std::map<std::string, int> {
        { "one", 1 }, { "two", 2 }, { "three", 3 }
    };

    // apply map function to all kinds of containers
    {
        auto v = funclib::mapf([] (int const i) {
                    return std::abs(i);
                }, vnums);
        recipe_common::print_collection(v, "v: ");

        auto lnums = std::list<int> { 1, 2, 3, 4, 5 };
        auto l = funclib::mapf([] (int const i) {
                    return i * i;
                }, lnums);
        recipe_common::print_collection(l, "l: ");

        auto amounts = std::array<double, 5> { 10.42, 2.50, 100.0, 23.75, 12.99 };
        auto a = funclib::mapf(fround<>(), amounts);
        recipe_common::print_collection(a, "a: ");

        auto m = funclib::mapf([] (std::pair<std::string, int> const kvp) {
                    return std::make_pair(funclib::mapf(toupper, kvp.first), kvp.second);
                }, words);
        recipe_common::print_collection(m, "m: ");

        auto priorities = std::queue<int>();
        priorities.push(10);
        priorities.push(20);
        priorities.push(30);
        priorities.push(40);
        priorities.push(50);
        auto p = funclib::mapf([] (int const i) {
                    return (i > 30) ? 2 : 1;
                }, priorities);
        recipe_common::print_queue(p, "p: ");
        std::cout << std::endl;
    }

    // apply fold function to all kinds of containers
    {
        auto s1 = funclib::foldl([] (const int s, const int n) {
                    return s + n;
                }, vnums, 0);
        std::cout << s1 << std::endl;

        auto s2 = funclib::foldl(std::plus<>(), vnums, 0);
        std::cout << s2 << std::endl;

        auto s3 = funclib::foldr([](const int s, const int n) {
                    return s + n;
                }, vnums, 0);
        std::cout << s3 << std::endl;

        auto s4 = funclib::foldr(std::plus<>(), vnums, 0);
        std::cout << s4 << std::endl;

        auto texts = std::vector<std::string> {
            "hello"s, " "s, "world"s, "!"s
        };
        auto txt1 = funclib::foldl([] (std::string const &s, std::string const &n) {
                    return s + n;
                }, texts, ""s);
        std::cout << txt1 << std::endl;
        auto txt2 = funclib::foldr([] (std::string const &s, std::string const &n) {
                    return s + n;
                }, texts, ""s);
        std::cout << txt2 << std::endl;

        char chars[] = { 'c', 'i', 'v', 'i', 'c' };
        auto str1 = funclib::foldl(std::plus<>(), chars, ""s);
        std::cout << str1 << std::endl;
        auto str2 = funclib::foldr(std::plus<>(), chars, ""s);
        std::cout << str2 << std::endl;

        auto count = funclib::foldl([] (int const s, std::pair<std::string, int> const kvp) {
                    return s + kvp.second;
                }, words, 0);
        std::cout << count << std::endl;

        auto q = std::queue<int>();
        q.push(1);
        q.push(2);
        q.push(3);
        q.push(4);
        q.push(5);
        auto sum1 = funclib::foldl(std::plus<>(), q, 0);
        std::cout << sum1 << std::endl;
        auto sum2 = funclib::foldr(std::plus<>(), q, 0);
        std::cout << sum2 << std::endl;
        std::cout << std::endl;
    }

    // pipeline map functions and fold functions
    {
        auto vnums = std::vector<int> { 0, 2, -3, 5, -1, 6, 8, -4, 9 };
        auto s = funclib::foldl(std::plus<>(),
                funclib::mapf([] (int const i) {
                            return i * i;
                        },
                        funclib::mapf([] (int const i) {
                            return std::abs(i);
                        }, vnums)),
                0);
        std::cout << s << std::endl;
        std::cout << std::endl;
    }

    // implement the fold function as a variadic function template
    {
        auto s1 = funclib::foldl_variadic(std::plus<>(), 1, 2, 3, 4, 5);
        std::cout << s1 << std::endl;
        auto s2 = funclib::foldl_variadic(std::plus<>(), "hello"s, ' ', "world"s, '!');
        std::cout << s2 << std::endl;
        // auto s3 = funclib::foldl_variadic(std::plus<>(), 1);  // error
        auto s4 = funclib::foldr_variadic(std::plus<>(), 1, 2, 3, 4, 5);
        std::cout << s4 << std::endl;
        auto s5 = funclib::foldr_variadic(std::plus<>(), "hello"s, ' ', "world"s, '!');
        std::cout << s5 << std::endl;
    }
}

}
