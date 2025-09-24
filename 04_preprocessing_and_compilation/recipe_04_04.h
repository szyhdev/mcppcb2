#pragma once

#include <iostream>

using namespace std::string_literals;

namespace recipe_04_04
{

template<bool Test, class T = void>
struct enable_if
{
};

template<class T>
struct enable_if<true, T>
{
    typedef T type;
};

template <class T>
typename enable_if<std::is_integral<T>::value, bool>::type is_odd(T i)    // return type
{
    return bool(i % 2);
}

template < class T,
        typename = typename enable_if<std::is_integral<T>::value>::type>  // template argument
bool is_even(T i)
{
    return !bool(i % 2);
}

template <typename T,
        typename = typename std::enable_if_t<std::is_trivial_v<T>, T>>
class pod_wrapper
{
    T value;
};

struct point
{
    int x;
    int y;
};

template <typename T,
        typename = typename std::enable_if_t<std::is_integral_v<T>, T>>
auto mul(T const a, T const b)
{
    return a * b;
}

template <typename Test, typename T = void>
using EnableIf = typename std::enable_if_t<Test::value, T>;

template <typename Test, typename T = void>
using DisableIf = typename std::enable_if_t<!Test::value, T>;

template <typename T, typename = EnableIf<std::is_trivial<T>>>
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
void func(typename T::value_type const a)
{
    std::cout << "func<typename T>(typename T::value_type const)" << std::endl;
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
    return a * b;
}

template <typename T, typename = DisableIf<std::is_integral<T>>,
        typename = void>
auto compute(T const a, T const b)
{
    return a + b;
}

void execute()
{
    // standard implementation of enable_if in C++11
    {
        std::cout << std::boolalpha;
        std::cout << is_odd(5) << ", " << is_even(5) << std::endl;
    }

    // enable a class template only for types that meet a specified condition
    {
        std::cout << std::is_trivial_v<int> << ", " <<
                std::is_trivial_v<point> << ", " <<
                std::is_trivial_v<std::string> << std::endl;

        pod_wrapper<int> pw1;             // OK
        pod_wrapper<point> pw2;           // OK
        // pod_wrapper<std::string> pw3;  // error: template argument 2 is invalid
    }

    // enable a function template only for types that meet a specified condition
    {
        auto v1 = mul(1, 2);         // OK
        // auto v2 = mul(1.0, 2.0);  // error: no matching overloaded function found
    }

    // a class template based on alias template EnableIf
    {
        pod_wrapper_alias<int> pwa1;
        pod_wrapper_alias<point> pwa2;
        // pod_wrapper_alias<std::string> pwa3;  // error: template argument 2 is invalid
    }

    // a function template based on alias template EnableIf
    {
        auto v1 = mul_alias(1, 2);         // OK
        // auto v2 = mul_alias(1.0, 2.0);  // error: no matching overloaded function
    }

    // have a quick look at what SFINAE is
    {
        func(42);                 // best match is void func(int const)
        func<some_type<int>>(42); // best match is void func(some_type<int>::value_type const>
        // func("string"s);       // error: no match for string argument
    }

    // static_assert is considered after overload functions have been resolved
    {
        // auto v1 = compute_ambiguous(1, 2);      // error: ambiguous call
        // auto v2 = compute_ambiguous(1.0, 2.0);  // error: ambiguous call
    }

    // discard the overload that produces a substitution failure with SFINAE
    {
        auto v1 = compute(1, 2);      // v1 = 2
        auto v2 = compute(1.0, 2.0);  // v2 = 3.0
    }
}

}
