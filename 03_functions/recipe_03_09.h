#pragma once

#include "recipe_03_09_common.h"

namespace recipe_03_09
{

using namespace recipe_03_09_common;

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
    auto ladd = [] (auto a, auto b) {
        return a + b;
    };

    // all types of callable objects
    {
        // function
        auto s1 = add(2, 3);
        std::cout << s1 << std::endl;

        // function pointer
        int (*fp1)(int, int) = &add;
        auto s2 = fp1(2, 3);
        std::cout << s2 << std::endl;

        auto fp2 = &add;
        auto s3 = fp2(2, 3);
        std::cout << s3 << std::endl;

        // member function pointer
        foo f;
        void (foo::*mfp1)(int) = &foo::increment;
        (f.*mfp1)(3);
        std::cout << f.x << std::endl;

        auto mfp2 = &foo::increment;
        (f.*mfp2)(3);
        std::cout << f.x << std::endl;

        // data member
        auto x = f.x;
        std::cout << x << std::endl;

        // function objects
        auto s4 = std::plus<>()(f.x, 3);
        std::cout << s4 << std::endl;

        // lambda expression
        auto s5 = ladd(2, 3);
        std::cout << s5 << std::endl;
        std::cout << std::endl;
    }

    // use std::invoke to take callable object as first argument and
    // a variable list of arguments that are passed to the call
    {
        // function
        auto s1 = std::invoke(add, 2, 3);
        std::cout << s1 << std::endl;

        // function pointer
        int (*fp1)(int, int) = &add;
        auto s2 = std::invoke(fp1, 2, 3);
        std::cout << s2 << std::endl;

        auto fp2 = &add;
        auto s3 = std::invoke(fp2, 2, 3);
        std::cout << s3 << std::endl;

        // member function pointer
        foo f;
        void (foo::*mfp1)(int) = &foo::increment;
        std::invoke(mfp1, f, 3);
        std::cout << f.x << std::endl;

        auto mfp2 = &foo::increment;
        std::invoke(mfp2, f, 3);
        std::cout << f.x << std::endl;

        // data member
        auto x = std::invoke(&foo::x, f);
        std::cout << x << std::endl;

        // function objects
        auto s4 = std::invoke(std::plus<>(), std::invoke(&foo::x, f), 3);
        std::cout << s4 << std::endl;

        // lambda expression
        auto s5 = std::invoke(ladd, 2, 3);
        std::cout << s5 << std::endl;
        std::cout << std::endl;
    }

    // a possible implementation of std::apply
    {
        auto s = recipe_03_09::apply(add, std::make_tuple(2, 3));
        std::cout << s << std::endl;
    }
}

}
