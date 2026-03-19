#pragma once

#include "recipe_03_common.h"

namespace recipe_03_07
{

using namespace std::string_literals;

template <typename T = double>
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

    // apply map higher-order functions to all kinds of containers
    {
        // retain absolute values from a vector
        auto v = recipe_03_common::mapf([] (int const i) {
                    return std::abs(i);
                }, vnums);
        recipe_common::print_collection(v, "v: ");

        // square the numerical values of a list
        auto lnums = std::list<int> { 1, 2, 3, 4, 5 };
        auto l = recipe_03_common::mapf([] (int const i) {
                    return i * i;
                }, lnums);
        recipe_common::print_collection(l, "l: ");

        // rounded amounts of floating points
        auto amounts = std::array<double, 5> { 10.42, 2.50, 100.0, 23.75, 12.99 };
        auto a = recipe_03_common::mapf(fround<>(), amounts);
        recipe_common::print_collection(a, "a: ");

        // uppercase the string keys of a map of words
        auto m = recipe_03_common::mapf([] (std::pair<std::string, int> const kvp) {
                    return std::make_pair(recipe_03_common::mapf(toupper, kvp.first), kvp.second);
                }, words);
        recipe_common::print_collection(m, "m: ");

        // find the opposite numbers of a set
        auto snums = std::set<int>(vnums.begin(), vnums.end());
        auto s = recipe_03_common::mapf([] (int const i) {
                    return -i;
                }, snums);
        recipe_common::print_collection(s, "s: ");

        // normalize values from a queue of priorities
        auto priorities = std::queue<int>();
        priorities.push(10);
        priorities.push(20);
        priorities.push(40);
        priorities.push(30);
        priorities.push(50);
        auto p = recipe_03_common::mapf([] (int const i) {
                    return (i > 30) ? 2 : 1;
                }, priorities);
        recipe_common::print_queue(p, "p: ");
        std::cout << std::endl;
    }

    // apply fold higher-order functions to all kinds of containers
    {
        // add the values of a vector of integers
        auto s1 = recipe_03_common::foldl([] (const int s, const int n) {
                    return s + n;
                }, vnums, 0);
        std::cout << s1 << std::endl;
        auto s2 = recipe_03_common::foldl(std::plus<>(), vnums, 0);
        std::cout << s2 << std::endl;
        auto s3 = recipe_03_common::foldr([](const int s, const int n) {
                    return s + n;
                }, vnums, 0);
        std::cout << s3 << std::endl;
        auto s4 = recipe_03_common::foldr(std::plus<>(), vnums, 0);
        std::cout << s4 << std::endl;

        // concatenate strings from a vector into a single string
        auto texts = std::vector<std::string> {
            "hello"s, " "s, "world"s, "!"s
        };
        auto txt1 = recipe_03_common::foldl([] (std::string const &s, std::string const &n) {
                    return s + n;
                }, texts, ""s);
        std::cout << txt1 << std::endl;
        auto txt2 = recipe_03_common::foldr([] (std::string const &s, std::string const &n) {
                    return s + n;
                }, texts, ""s);
        std::cout << txt2 << std::endl;

        // concatenate an array of characters into a string
        char chars[] = { 'l', 'i', 'n', 'u', 'x' };
        auto str1 = recipe_03_common::foldl(std::plus<>(), chars, ""s);
        std::cout << str1 << std::endl;
        auto str2 = recipe_03_common::foldr(std::plus<>(), chars, ""s);
        std::cout << str2 << std::endl;

        // count the number of words in text based on std::map<std::string, int>
        auto count = recipe_03_common::foldl([] (int const s,
                std::pair<std::string, int> const kvp) {
                    return s + kvp.second;
                }, words, 0);
        std::cout << count << std::endl;

        // add the values of a queue of integers
        auto q = std::queue<int>();
        q.push(1);
        q.push(2);
        q.push(3);
        q.push(4);
        q.push(5);
        auto sum = recipe_03_common::foldl(std::plus<>(), q, 0);
        std::cout << sum << std::endl;
        std::cout << std::endl;
    }

    // pipeline map functions and fold functions
    {
        auto s = recipe_03_common::foldl(std::plus<>(),
                recipe_03_common::mapf([] (int const i) {
                            return i * i;
                        }, recipe_03_common::mapf([] (int const i) {
                            return std::abs(i);
                        }, vnums)),
                0);
        std::cout << s << std::endl;
        std::cout << std::endl;
    }

    // implement the fold function as a variadic function template
    {
        auto s1 = recipe_03_common::foldl_v(std::plus<>(), 1, 2, 3, 4, 5);
        std::cout << s1 << std::endl;
        auto s2 = recipe_03_common::foldl_v(std::plus<>(), "hello"s, ' ', "world"s, '!');
        std::cout << s2 << std::endl;
        auto s3 = recipe_03_common::foldr_v(std::plus<>(), 1, 2, 3, 4, 5);
        std::cout << s3 << std::endl;
        auto s4 = recipe_03_common::foldr_v(std::plus<>(), "hello"s, ' ', "world"s, '!');
        std::cout << s4 << std::endl;
        // auto s5 = recipe_03_common::foldl_v(std::plus<>(), 1);  // error
    }
}

}
