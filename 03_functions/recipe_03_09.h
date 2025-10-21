#pragma once

#include "recipe_03_common.h"

#include <functional>

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

namespace details
{
    template <class F, class T, std::size_t... I>
    auto apply(F &&f, T &&t, std::index_sequence<I...>)
    {
        return std::invoke(std::forward<F>(f),
                std::get<I>(std::forward<T>(t))...);
    }
}

template <class F, class T>
auto apply(F &&f, T &&t)
{
    return details::apply(std::forward<F>(f), std::forward<T>(t),
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

        auto a3 = std::invoke(fadd1, 1, 2);
        std::cout << a3 << std::endl;

        auto a4 = std::invoke(fadd2, 1, 2);
        std::cout << a4 << std::endl;
        std::cout << std::endl;
    }

    // member functions through pointer to member function
    {
        foo f1;
        void (foo::*finc)(int const) = &foo::increment_by;
        (f1.*finc)(3);
        std::cout << f1.x << std::endl;

        auto finc2 = &foo::increment_by;
        (f1.*finc2)(3);
        std::cout << f1.x << std::endl;

        foo f2;
        std::invoke(&foo::increment_by, f2, 3);
        auto x1 = std::invoke(&foo::x, f2);
        std::cout << x1 << std::endl;

        std::invoke(finc2, f2, 3);
        auto x2 = std::invoke(&foo::x, f2);
        std::cout << x2 << std::endl;
        std::cout << std::endl;
    }

    // data members
    {
        foo f1;
        f1.increment_by(3);
        std::cout << f1.x << std::endl;

        foo f2;
        f2.increment_by(3);
        auto x1 = std::invoke(&foo::x, f2);
        std::cout << x1 << std::endl;
        std::cout << std::endl;
    }

    // function objects
    {
        foo f1;
        auto x1 = std::plus<>()(std::invoke(&foo::x, f1), 3);
        std::cout << x1 << std::endl;

        foo f2;
        auto x2 = std::invoke(std::plus<>(),
                std::invoke(&foo::x, f2), 3);
        std::cout << x2 << std::endl;
        std::cout << std::endl;
    }

    // lambda expressions
    {
        auto l = [] (auto a, auto b) {
            return a + b;
        };

        auto a = std::invoke(l, 1, 2);
        std::cout << a << std::endl;
        std::cout << std::endl;
    }

    {
        // use namespace to avoid finding std::apply
        auto a = recipe_03_09::apply(add, std::make_tuple(1, 2));
        std::cout << a << std::endl;
    }
}

}
