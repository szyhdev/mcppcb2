#pragma once

#include "recipe_04_common.h"

namespace recipe_04_04
{

template <typename T>
void func(typename T::value_type const a)
{
    std::cout << "func<T>(T::value_type const)" << std::endl;
}

void func(int const a)
{
    std::cout << "func(int const)" << std::endl;
}

template <typename T>
struct some_type
{
    using value_type = T;
};

template <bool Test, typename T = void>
struct enable_if
{
};

template <typename T>
struct enable_if<true, T>
{
    typedef T type;
};

struct point
{
    int x;
    int y;
};

template <typename T,
        // typename = std::enable_if_t<std::is_standard_layout_v<T> && std::is_trivial_v<T>, T>>
        // typename = typename std::enable_if<std::is_standard_layout_v<T>, T>::type>
        typename = std::enable_if_t<std::is_standard_layout_v<T>, T>>
class pod_wrapper
{
    T value;
};

template <typename T>
concept standard_layout = std::is_standard_layout_v<T>;

template <typename T>
concept trivial = std::is_trivial_v<T>;

template <typename T>
concept pod_type = standard_layout<T> && trivial<T>;

template <pod_type T>
class pod_wrapper_concept
{
    T value;
};

template <typename T>
class pod_wrapper_static_assert
{
    static_assert(std::is_standard_layout_v<T> && std::is_trivial_v<T>,
            "T must be standard layout");
    T value;
};

template <typename T,
        typename = std::enable_if_t<std::is_integral_v<T>, T>>
auto mul(T const a, T const b)
{
    return a * b;
}

template <typename T>
std::enable_if_t<std::is_integral_v<T>, bool> is_odd(T i)
{
    return bool(i % 2);
}

template <typename Test, typename T = void>
using EnableIf = std::enable_if_t<Test::value, T>;

template <typename Test, typename T = void>
using DisableIf = std::enable_if_t<!Test::value, T>;

template <typename T, typename = EnableIf<std::is_standard_layout<T>>,
        typename = EnableIf<std::is_trivial<T>>>
class pod_wrapper_alias
{
    T value;
};

template <typename T, typename = EnableIf<std::is_integral<T>>>
auto mul_alias(T const a, T const b)
{
    return a * b;
}

template <typename T>
auto compute_ambiguous(T const a, T const b)
{
    static_assert(std::is_integral_v<T>, "An integral type expected");
    return a + b;
}

template <typename T, typename = void>
auto compute_ambiguous(T const a, T const b)
{
    static_assert(!std::is_integral_v<T>, "A non-integral type expected");
    return a * b;
}

template <typename T, typename = EnableIf<std::is_integral<T>>>
auto compute(T const a, T const b)
{
    return a + b;
}

template <typename T, typename = DisableIf<std::is_integral<T>>,
        typename = void>
auto compute(T const a, T const b)
{
    return a * b;
}

void execute()
{
    // show how SFINAE works
    {
        func(42);                        // OK
        func<some_type<int>>(42);        // OK
        // func(std::string("string"));  // error: no match for string argument
        std::cout << std::endl;
    }

    // enable a class template only for specific types
    {
        pod_wrapper<int> pw1;             // OK
        pod_wrapper<point> pw2;           // OK
        // pod_wrapper<std::string> pw3;  // error: too few template arguments
    }

    // enable a function template only for specific types
    {
        auto v1 = mul(1, 2);         // OK
        // auto v2 = mul(1.0, 2.0);  // error: no matching overloaded function found

        std::cout << std::boolalpha << is_odd(5) << std::endl;
    }

    // class template based on alias template EnableIf
    {
        pod_wrapper_alias<int> pwa1;
        pod_wrapper_alias<point> pwa2;
        // pod_wrapper_alias<std::string> pwa3;  // error: too few template arguments
    }

    // function template based on alias template EnableIf
    {
        auto v1 = mul_alias(1, 2);         // OK
        // auto v2 = mul_alias(1.0, 2.0);  // error: no matching overloaded function
    }

    // discard the overload that produces a substitution failure
    {
        // auto v1 = compute_ambiguous(1, 2);      // error: ambiguous call to overloaded function
        // auto v2 = compute_ambiguous(1.0, 2.0);  // error: ambiguous call to overloaded function
        auto v3 = compute(1, 2);                   // OK
        auto v4 = compute(1.0, 2.0);               // OK
    }
}

}
