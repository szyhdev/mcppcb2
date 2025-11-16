#pragma once

#include "recipe_09_common.h"

namespace recipe_09_02
{

void func_no_throw() noexcept
{
}

template <typename T>
T generic_func_1() noexcept(std::is_nothrow_constructible_v<T>)
{
    return T {};
}

template <typename T>
T generic_func_2() noexcept(noexcept(T {}))
{
    return T {};
}

template <typename F, typename A>
auto func(F &&f, A &&arg) noexcept
{
    static_assert(noexcept(f(arg)), "F is throwing!");
    return f(arg);
}

int double_it(int const i) noexcept
{
    return i + i;
}

int half_it(int const i)
{
    throw std::runtime_error("not implemented!");
}

struct foo
{
    foo() {
    }
};

struct may_throw_move
{
    std::string data;

    may_throw_move(const std::string &s) : data(s) {
        std::cout << "may_throw_move: constructor" << std::endl;
    }

    may_throw_move(may_throw_move const &other) noexcept : data(other.data) {
        std::cout << "may_throw_move: copy constructor" << std::endl;
    }

    may_throw_move(may_throw_move &&other) : data(std::move(other.data)) {
        std::cout << "may_throw_move: move constructor" << std::endl;
    }
};

struct no_throw_move
{
    std::string data;

    no_throw_move(const std::string &s) : data(s) {
        std::cout << "no_throw_move: constructor" << std::endl;
    }

    no_throw_move(no_throw_move const &other) noexcept : data(other.data) {
        std::cout << "no_throw_move: copy constructor" << std::endl;
    }

    no_throw_move(no_throw_move &&other) noexcept : data(std::move(other.data)) {
        std::cout << "no_throw_move: move constructor" << std::endl;
    }
};

template <typename T>
void safe_relocate(std::vector<T> &vec)
{
    std::vector<T> new_vec;
    new_vec.reserve(vec.size());

    for (auto &elem : vec) {
        new_vec.push_back(std::move_if_noexcept(elem));
    }

    vec.swap(new_vec);
}

void execute()
{
    {
        std::cout << std::boolalpha;
        std::cout << noexcept(func_no_throw()) << std::endl;
        std::cout << noexcept(generic_func_1<int>()) << std::endl;
        std::cout << noexcept(generic_func_1<std::string>()) << std::endl;
        std::cout << noexcept(generic_func_2<int>()) << std::endl;
        std::cout << noexcept(generic_func_2<std::string>()) << std::endl;
        std::cout << noexcept(generic_func_2<foo>()) << std::endl;
        std::cout << noexcept(double_it(42)) << std::endl;
        std::cout << noexcept(half_it(42)) << std::endl;
        std::cout << noexcept(func(double_it, 42)) << std::endl;
        // std::cout << noexcept(func(half_it, 42)) << std::endl;  // static assertion
        std::cout << std::endl;
    }

    {
        std::vector<may_throw_move> vec1;
        vec1.reserve(2);
        vec1.emplace_back("mtm1");
        vec1.emplace_back("mtm2");
        safe_relocate(vec1);  // copy constructor

        std::vector<no_throw_move> vec2;
        vec2.reserve(2);
        vec2.emplace_back("ntm1");
        vec2.emplace_back("ntm2");
        safe_relocate(vec2);  // move constructor
    }
}

}
