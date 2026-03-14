#pragma once

#include "recipe_03_common.h"

#include <stdarg.h>

namespace recipe_03_05
{
using namespace std::string_literals;

#define LOG(...) {                                          \
    fprintf(stderr, "%s: Line %d:\t", __FILE__, __LINE__);  \
    fprintf(stderr, __VA_ARGS__);                           \
    fprintf(stderr, "\n");                                  \
}

double average(int count, ...)
{
    va_list args;
    va_start(args, count);
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        double num = va_arg(args, double);
        sum += num;
    }
    va_end(args);
    return (count > 0) ? sum / count : 0.0;
}

namespace v1
{
// #define SHOW_OVERLOADED_FUNCTIONS

template <typename T>
T add(T value)
{
#ifdef SHOW_OVERLOADED_FUNCTIONS
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    return value;
}

template <typename T, typename ...Ts>
T add(T head, Ts... rest)
{
#ifdef SHOW_OVERLOADED_FUNCTIONS
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    return head + add(rest...);
}

}

namespace v2
{

template <typename T>
auto add(T value)
{
    return value;
}

template <typename T, typename ...Ts>
auto add(T head, Ts... rest)
{
    return head + add(rest...);
}

}

template <typename ...T>
auto make_even_tuple(T... a)
{
    static_assert(sizeof...(a) % 2 == 0,
            "expected an even number of arguments");
    std::tuple<T...> t { a... };
    return t;
}

template <typename T>
bool is_any(T value, T elem)
{
    return value == elem;
}

template <typename T, typename ...Ts>
bool is_any(T value, T first, Ts... rest)
{
    return (value == first) || is_any(value, rest...);
}

namespace v1
{

template <size_t N, typename ...T>
void print_tuple_impl(const std::tuple<T...> &t) {
    if constexpr (N < std::tuple_size_v<std::tuple<T...>>) {
        std::cout << std::get<N>(t) << " ";
        print_tuple_impl<N + 1>(t);
    }
}

template <typename ...T>
void print_tuple(const std::tuple<T...> &t)
{
    print_tuple_impl<0>(t);
    std::cout << std::endl;
}

}

namespace v2
{

template <std::size_t N, typename ...T>
struct tuple_printer
{
    static void print(const std::tuple<T...> &t) {
        if constexpr (N < std::tuple_size_v<std::tuple<T...>>) {
            std::cout << std::get<N>(t) << " ";
            tuple_printer<N + 1, T...>::print(t);
        }
    }
};

template <typename ...T>
void print_tuple(const std::tuple<T...> &t)
{
    tuple_printer<0, T...>::print(t);
    std::cout << std::endl;
}

}

void execute()
{
    // C variadic function
    {
        double avg = average(3, 1.5, 2.5, 3.5);
        std::cout << avg << std::endl;
        std::cout << std::endl;
    }

    // C variadic macro
    {
        int x = 3;
        LOG("x = %d", x);
        std::cout << std::endl;
    }

    // a variadic function template that adds a variable number of arguments
    {
        auto n = v1::add(1, 2, 3, 4, 5);
        std::cout << n << std::endl;
        auto s = v1::add("hello"s, " "s, "world"s, "!"s);
        // auto s = add("hello"s, ' ', "world"s, '!');  // error
        std::cout << s << std::endl;
        std::cout << std::endl;
    }

    // a variadic function template with auto as the return type
    {
        auto n2 = v2::add(1, 2, 3, 4, 5);
        std::cout << n2 << std::endl;
        auto s2 = v2::add("hello"s, ' ', "world"s, '!');
        std::cout << s2 << std::endl;
        std::cout << std::endl;
    }

    // a parameter pack that appears in a brace-initialization and
    // whose size can be determined by sizeof... operator
    {
        auto t = make_even_tuple(1, 'a', 3.14, "hello");
        // auto t = make_even_tuple(1, 'a', 3.14);  // error
        v1::print_tuple(t);
        v2::print_tuple(t);
        std::cout << std::endl;
    }

    // a variadic function template that check the existence of a value
    {
        auto is = is_any(42, 1, 23, 76, 42, 5);
        std::cout << std::boolalpha << is << std::endl;
    }
}

}
