#pragma once

#include "recipe_01_common.h"

namespace recipe_01_01
{

using namespace std::string_literals;

#if !defined(USE_TEMPLATE)

auto func(auto x, auto y)
{
    std::cout << "func(auto, auto)" << std::endl;
    return x + y;
}

#else

template <typename T, typename U>
auto func(T a, U b)
{
    std::cout << "func(T, U)" << std::endl;
    return a + b;
}

#endif

class foo
{
    int x_;

public:
    foo(int const x = 0) : x_ { x } {
    }

    int &get() {
        return x_;
    }

    int get() const {
        return x_;
    }
};

namespace v1
{
    auto make_double(int const i) -> int
    {
        return 2 * i;
    }
}

namespace v2
{
    auto make_double(int const i)
    {
        return 2 * i;
    }
}

// wrong version
// auto proxy_get(foo &f)
// {
//     return f.get();
// }

// right version
auto &proxy_get(foo &f)
{
    return f.get();
}

// all-purpose version
template <typename T>
decltype(auto) proxy_get(T &&f)
{
    return f.get();
}

int get_int(int value)
{
    return value;
}

double get_double(double value)
{
    return value;
}

template <typename F, typename T>
auto apply(F &&f, T value)
{
    return f(value);
}

struct
{
    template <typename T, typename U>
    auto operator ()(T const a, U const b) const {
        return a + b;
    }
} sadd;

void execute()
{
    // usage 1: define local variables
    {
        // in the form of "auto name = expression"
        auto i = 42;            // int
        auto d = 42.5;          // double
        auto s1 = "text";       // char const *
        auto v1 = { 1, 2, 3 };  // std::initializer_list<int>

        // in the form of "auto name = type-id { expression }"
        auto ar = new char[10] { 0 };            // char *
        auto s2 = std::string { "text" };        // std::string
        auto v2 = std::vector<int> { 1, 2, 3 };  // std::vector<int>
        auto p = std::make_shared<int>(42);      // std::shared_ptr<int>
    }

    // usage 2: declare named lambda functions
    {
        // in the form "auto name = lambda-expression"
        auto upper = [] (char const c) {
            return toupper(c);
        };
    }

    // usage 3: declare lambda parameters and return values
    {
        auto add = [] (auto const a, auto const b) {
            return a + b;
        };
    }

    // usage 4: declare a function return type
    {
        auto res1 = apply(get_int, 100);
        std::cout << res1 << std::endl;
        auto res2 = apply(get_double, 3.14);
        std::cout << res2 << std::endl;
        std::cout << std::endl;
    }

    // advantage 2: use correct type and implicit conversion won't occur
    {
        auto v = std::vector<int> { 1, 2, 3 };
        int size1 = v.size();              // implicit conversion
        auto size2 = v.size();             // OK
        // auto size3 = int { v.size() };  // warning: narrowing conversion
    }

    // advantage 4: less typing and less concern for actual types
    {
        std::map<int, std::string> m {
            { 1, "one" },
            { 2, "two" },
            { 3, "three" }
        };

        for (std::map<int, std::string>::const_iterator itr = m.cbegin();
                itr != m.cend(); ++itr) {
            std::cout << "(" << itr->first << ", " << itr->second << ") ";
        }
        std::cout << std::endl;

        for (auto itr = m.cbegin(); itr != m.cend(); ++itr) {
            std::cout << "(" << itr->first << ", " << itr->second << ") ";
        }
        std::cout << std::endl;

        for (auto &e : m) {
            std::cout << "(" << e.first << ", " << e.second << ") ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }

    // advantage 5: provides a consistent coding style with type always on the right side
    {
        int *p1 = new int(42);
        delete p1;
        auto p2 = new int(42);
        delete p2;
    }

    // note 1: auto is a placeholder for type, not for const/volatile and references specifiers
    {
        foo f(42);

        auto x = f.get();
        x = 100;
        std::cout << x << ", " << f.get() << std::endl;

        auto &y = f.get();
        y = 100;
        std::cout << y << ", " << f.get() << std::endl;
        std::cout << std::endl;
    }

    // note 2: don't use auto for types that are not moveable
    {
        auto ai = std::atomic<int>(42);  // error in c++11/14
        std::atomic<int> ai1(42);        // OK
        std::atomic<int> ai2 { 42 };     // OK
    }

    // note 3: don't use auto for multi-word types, such as long long
    {
        // auto l1 = long long { 42 };  // error
        using llong = long long;
        auto l2 = llong{42};            // OK
        auto l3 = 42LL;                 // OK
    }

    // scenario 1: specify return type of a function
    {
        auto x = v1::make_double(3);  // a trailing return type required in C++11
        auto y = v2::make_double(4);  // the return value type deduced by the compiler in C++14
    }

    // scenario 2: deduce return type of a wrapper function, value or reference
    {
        foo f1(42);
        auto &p1 = proxy_get(f1);
        static_assert(std::is_same_v<decltype(p1), int &>);
        decltype(auto) p2 = proxy_get(f1);
        static_assert(std::is_same_v<decltype(p2), int &>);

        foo const f2(42);
        decltype(auto) p3 = proxy_get(f2);
        static_assert(std::is_same_v<decltype(p3), int>);
    }

    // scenario 3: use auto with generic lambda
    {
        auto ladd = [] (auto const a, auto const b) {
            return a + b;
        };

        auto i1 = ladd(40, 2);
        auto s1 = ladd("forty"s, " two"s);
        auto i2 = sadd(40, 2);
        auto s2 = sadd("forty"s, " two"s);
        std::cout << i1 << ", " << s1 << std::endl;
        std::cout << i2 << ", " << s2 << std::endl;
    }
}

}
