#pragma once

#include "recipe_03_common.h"

namespace recipe_03_09
{

int add(int const a, int const b)
{
    return a + b;
}

struct foo
{
    int x = 0;

    void increment_by(int const n) {
        x += n;
    }
};

template <typename F, typename T, std::size_t ...I>
auto apply(F &&f, T &&t, std::index_sequence<I...>)
{
    return std::invoke(std::forward<F>(f),
            std::get<I>(std::forward<T>(t))...);
}

template <typename F, typename T>
auto apply(F &&f, T &&t)
{
    return apply(std::forward<F>(f), std::forward<T>(t),
            std::make_index_sequence<std::tuple_size_v<std::decay_t<T>>> {});
}

void execute()
{
    // free functions
    {
        auto a1 = add(1, 2);
        std::cout << a1 << std::endl;

        auto a2 = std::invoke(add, 1, 2);
        std::cout << a2 << std::endl;
        std::cout << std::endl;
    }

    // free functions through pointer to function
    {
        int (*fadd1)(int const, int const) = &add;
        auto a1 = fadd1(1, 2);
        std::cout << a1 << std::endl;

        auto fadd2 = &add;
        auto a2 = fadd2(1, 2);
        std::cout << a2 << std::endl;

        auto a3 = std::invoke(&add, 1, 2);
        std::cout << a3 << std::endl;

        auto a4 = std::invoke(fadd1, 1, 2);
        std::cout << a4 << std::endl;

        std::cout << std::endl;
    }

    // member functions through pointer to member function
    {
        foo f;
        f.increment_by(3);
        std::cout << f.x << std::endl;

        void (foo::*finc1)(int const) = &foo::increment_by;
        (f.*finc1)(3);
        std::cout << f.x << std::endl;

        auto finc2 = &foo::increment_by;
        (f.*finc2)(3);
        std::cout << f.x << std::endl;

        std::invoke(&foo::increment_by, f, 3);
        std::cout << f.x << std::endl;

        std::invoke(finc1, f, 3);
        std::cout << f.x << std::endl;
        std::cout << std::endl;
    }

    // data members
    {
        foo f;
        f.increment_by(3);
        std::cout << f.x << std::endl;

        auto x = std::invoke(&foo::x, f);
        std::cout << x << std::endl;
        std::cout << std::endl;
    }

    // function objects
    {
        foo f;
        auto x1 = std::plus<>()(std::invoke(&foo::x, f), 3);
        std::cout << x1 << std::endl;

        auto x2 = std::invoke(std::plus<>(), std::invoke(&foo::x, f), 3);
        std::cout << x2 << std::endl;
        std::cout << std::endl;
    }

    // lambda expression
    {
        auto ladd = [] (auto a, auto b) {
            return a + b;
        };
        auto a1 = ladd(1, 2);
        std::cout << a1 << std::endl;

        auto a2 = std::invoke(ladd, 1, 2);
        std::cout << a2 << std::endl;
        std::cout << std::endl;
    }

    // a possible implementation for std::apply
    {
        auto a = recipe_03_09::apply(add, std::make_tuple(1, 2));
        std::cout << a << std::endl;
    }
}

}
