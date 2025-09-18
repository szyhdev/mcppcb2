#pragma once

#include <iostream>
#include <atomic>
#include <map>
#include <memory>
#include <vector>

namespace recipe_01_01
{

using namespace std::string_literals;

int print_value(int value)
{
    return value;
}

template <typename F, typename T>
auto apply(F &&f, T value) {
    return f(value);
}

class foo
{
    int x_;

public:
    foo(int const x = 0) : x_ { x } {
    }

    int &get() {
        return x_;
    }
};

auto proxy_get_wrong(foo &f)
{
    return f.get();
}

decltype(auto) proxy_get_right(foo &f)
{
    return f.get();
}

auto func1(int const i) -> int
{
    return 2 * i;
}

auto func2(int const i)
{
    return 2 * i;
}

struct
{
    template <typename T, typename U>
    auto operator()(T const a, U const b) const {
        return a + b;
    }
} stadd;

void execute()
{
    // declare local variables with the form 'auto name = expression'
    {
        auto i = 42;           // int
        auto d = 42.5;         // double
        auto s = "text";       // char const *
        auto v = { 1, 2, 3 };  // std::initializer_list<int>
    }

    // declare local variables with the form 'auto name = type-id { expression }'
    {
        auto b = new char[10] { 0 };            // char *
        auto s = std::string { "text" };        // std::string
        auto v = std::vector<int> { 1, 2, 3 };  // std::vector<int>
        auto p = std::make_shared<int>(42);     // std::shared_ptr<int>
    }

    // declare named lambda functions with the form 'auto name = lambda-expression'
    {
        auto upper = [] (char const c) {
            return toupper(c);
        };
    }

    // declare lambda parameters and return values
    {
        auto add = [] (auto const a, auto const b) {
            return a + b;
        };
    }

    // declare a function return type
    {
        auto result = apply(print_value, 100);
        std::cout << result << std::endl;
    }

    // use the correct type and implicit conversion won't occur
    {
        auto v = std::vector<int> { 1, 2, 3 };

        int size1 = v.size();              // implicit conversion
        auto size2 = v.size();             // OK
        // auto size3 = int { v.size() };  // warning: narrowing conversion
    }

    // less typing and less concern for actual types
    {
        std::map<int, std::string> m {
            { 1, "one" },
            { 2, "two" },
            { 3, "three" }
        };

        for (std::map<int, std::string>::const_iterator it = m.cbegin();
                it != m.cend(); ++it) {
            std::cout << "(" << it->first << ", " << it->second << ") ";
        }
        std::cout << std::endl;

        for (auto it = m.cbegin(); it != m.cend(); ++it) {
            std::cout << "(" << it->first << ", " << it->second << ") ";
        }
        std::cout << std::endl;
    }

    // provides a consistent coding style with the type always on the right side
    {
        int *p1 = new int(42);
        delete p1;
        auto p2 = new int(42);
        delete p2;
    }

    // auto is a placeholder for the type, not for the const/volatile and references specifiers
    {
        foo f(42);

        auto x = f.get();
        x = 100;
        std::cout << f.get() << ", " << x << std::endl;  // 42, 100

        auto &y = f.get();
        y = 100;
        std::cout << f.get() << ", " << y << std::endl;  // 100, 100
    }

    // don't use auto for types that are not moveable
    {
        auto ai = std::atomic<int>(42);  // error in c++11/14
    }

    // don't use auto for multi-word types, such as long long
    {
        // auto l1 = long long { 42 };  // error
        using llong = long long;
        auto l2 = llong{42};            // OK
        auto l3 = 42LL;                 // OK
    }

    // specify the return type from a function
    {
        auto x = func1(3);  // a trailing return type required in C++11
        auto y = func2(4);  // the return value type deduced by the compiler in C++14
    }

    // deduce the return type of a wrapper function, whether it is a value or a reference
    {
        auto f1 = foo { 42 };
        // auto &x = proxy_get_wrong(f1);        // error: cannot convert from 'int' to 'int &'

        auto f2 = foo { 42 };
        decltype(auto) y = proxy_get_right(f2);  // 'decltype(auto)' introduced in C++14
    }

    // use auto with generic lambdas
    {
        auto ladd = [] (auto const a, auto const b) {
            return a + b;
        };

        auto i = ladd(40, 2);             // 42
        auto s = ladd("forty"s, "two"s);  // "fortytwo"s
        auto x = stadd(12, 34);           // 46
        std::cout << i << ", " << s << ", " << x << std::endl;
    }
}

}
