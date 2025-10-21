#pragma once

#include "recipe_03_common.h"

// #define SHOW_OVERLOADED_FUNCTIONS

namespace recipe_03_05
{

using namespace std::string_literals;

template <typename T>
T add(T value)
{
#ifdef SHOW_OVERLOADED_FUNCTIONS
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    return value;
}

template <typename T, typename... Ts>
T add(T head, Ts... tail)
{
#ifdef SHOW_OVERLOADED_FUNCTIONS
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    return head + add(tail...);
}

template <typename T>
auto auto_add(T value)
{
    return value;
}

template <typename T, typename... Ts>
auto auto_add(T head, Ts... tail)
{
    return head + auto_add(tail...);
}

template <typename... T>
auto make_even_tuple(T... a)
{
    static_assert(sizeof...(a) % 2 == 0,
            "expected an even number of arguments");
    std::tuple<T...> t { a... };
    return t;
}

template <typename Tuple, std::size_t N>
struct tuple_printer
{
    static void print(const Tuple &t) {
        tuple_printer<Tuple, N - 1>::print(t);
        std::cout << std::get<N - 1>(t) << ' ';
    }
};

template <typename Tuple>
struct tuple_printer<Tuple, 1>
{
    static void print(const Tuple &t) {
        std::cout << std::get<0>(t) << ' ';
    }
};

template <typename T>
bool is_any(T value, T elem)
{
    return value == elem;
}

template <typename T, typename... Ts>
bool is_any(T value, T first, Ts... rest)
{
    return (value == first) || is_any(value, rest...);
}

void execute()
{
    // a variadic function template that adds a variable number of arguments
    {
        auto n = add(1, 2, 3, 4, 5);
        std::cout << n << std::endl;
        auto s = add("hello"s, " "s, "world"s, "!"s);
        std::cout << s << std::endl;
        std::cout << std::endl;
    }

    // modify the variadic function template so that it has auto
    // for the return type instead of T
    {
        auto n2 = auto_add(1, 2, 3, 4, 5);
        std::cout << n2 << std::endl;
        auto s2 = auto_add("hello"s, ' ', "world"s, '!');
        std::cout << s2 << std::endl;
        std::cout << std::endl;
    }

    // a parameter pack that appears in a brace-initialization and
    // its size can be determined using sizeof... operator
    {
        auto t1 = make_even_tuple(1, 2, 3, 4);
        tuple_printer<decltype(t1), std::tuple_size<decltype(t1)>::value>::print(t1);
        // auto t2 = make_even_tuple(1, 2, 3);  // error
        std::cout << std::endl << std::endl;
    }

    // an implementation of is_any function template
    {
        auto is = is_any(42, 1, 23, 76, 44, 5);
        std::cout << std::boolalpha << is << std::endl;
    }
}

}
