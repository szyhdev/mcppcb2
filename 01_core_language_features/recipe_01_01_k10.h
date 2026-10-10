#pragma once

#include "recipe_01_common.h"

namespace recipe_01_01_k10
{

using namespace std::string_literals;

void func()
{
    // auto int x = 0;
    int x = 0;
}

auto add_int = [] (int x, int y) -> int
{
    return x + y;
};

auto add_auto = [] (auto const a, auto const b)
{
    return a + b;
};

struct
{
    template <typename T, typename U>
    auto operator ()(T const a, U const b) const {
        return a + b;
    }
} add_type;

#if !defined(USE_TEMPLATE)

auto func(auto x, auto y)
{
    return x + y;
}

#else

template <typename T, typename U>
auto func(T a, U b)
{
    return a + b;
}

#endif

double get_double()
{
    return 3.14;
}

void func_iterator()
{
    std::vector<std::string> names { "zhangsan"s, "lisi"s, "wangwu"s };

    for (std::vector<std::string>::const_iterator itr = names.begin();
            itr != names.end(); ++itr) {
        std::cout << *itr << " ";
    }
    std::cout << std::endl;
}

void func_auto()
{
    std::vector<std::string> names { "zhangsan"s, "lisi"s, "wangwu"s };

    for (auto itr = names.begin(); itr != names.end(); ++itr) {
        std::cout << *itr << " ";
    }
    std::cout << std::endl;
}

void func_auto_range()
{
    std::vector<std::string> names { "zhangsan", "lisi", "wangwu" };

    for (auto const &n: names) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
}

namespace v1
{
    constexpr float PI = 3.1415927f;
}

namespace v2
{
    constexpr double PI = 3.1415927;
}

namespace v11
{
    template <typename T1, typename T2>
    auto sum(T1 &t1, T2 &t2) -> decltype(t1 + t2)
    {
        return t1 + t2;
    }
}

namespace v14
{
    template <typename T1, typename T2>
    auto sum(T1 &t1, T2 &t2)
    {
        return t1 + t2;
    }
}

namespace v20
{
    auto sum(auto &t1, auto &t2)
    {
        return t1 + t2;
    }
}

#define Max1(a, b) ((a) > (b)) ? (a) : (b)

#define Max2(a, b) ({ \
    auto _a = (a); \
    auto _b = (b); \
    (_a > _b) ? _a : _b; \
})

double func1()
{
    return 3.1415927;
}

float *func2()
{
    static float a = 6.18f;
    return &a;
}

int &func3()
{
    static int a = 123;
    return a;
}

const int &func4()
{
    static int a = 123;
    return a;
}

// void func(auto x = 1)  // error
// {
// }

// struct st
// {
//     auto id = 10;      // error
// };

void execute()
{
    // history of auto
    {
        func();

        auto i1 = add_int(1, 2);
        auto i2 = add_auto(1, 2);
        auto i3 = add_type(1, 2);
        // auto s1 = add_int("zhang"s, "san"s);  // error
        auto s2 = add_auto("zhang"s, "san"s);
        auto s3 = add_type("zhang"s, "san"s);

        auto [id, name] = std::make_tuple(1, "zhangsan"s);
        std::cout << "(" << id << ", " << name << ")" << std::endl;

        std::cout << func(1, 2) << std::endl;
        std::cout << func("zhang"s, "san"s) << std::endl;
        std::cout << std::endl;
    }

    // basic usages
    {
        auto x = 1;
        static_assert(std::is_same_v<decltype(x), int>);

        auto y = get_double();
        static_assert(std::is_same_v<decltype(y), double>);

        struct S {
            int i;
        } s1;
        static_assert(std::is_same_v<decltype(s1), S>);
        auto s2 = s1;
        static_assert(std::is_same_v<decltype(s2), S>);

        // auto z;  // error
        auto z = x;
        static_assert(std::is_same_v<decltype(z), int>);
    }

    // advantage 1: simplify type declaration
    {
        func_iterator();
        func_auto();
        func_auto_range();
        std::cout << std::endl;
    }

    // advantage 2: deduce type automatically and correctly
    {
        // version 1
        float radius1 = 1.5e10;
        float circumference1 = 2 * v1::PI * radius1;
        std::cout << std::fixed << circumference1 << std::endl;

        // version 2
        double radius2 = 1.5e10;
        auto circumference2 = 2 * v2::PI * radius2;
        static_assert(std::is_same_v<decltype(circumference2), double>);
        std::cout << std::fixed << circumference2 << std::endl;
        std::cout << std::endl;
    }

    // advantage 3: support generic programming
    {
        int i = 12;
        long l = 34;
        auto s1 = v20::sum(i, l);
        static_assert(std::is_same_v<decltype(s1), long>);

        float f1 = 3.14;
        float f2 = 6.18;
        auto s2 = v20::sum(f1, f2);
        static_assert(std::is_same_v<decltype(s2), float>);
    }

    // advantage 4: support macro definition
    {
        int m1 = Max1(1 * 2, 3 + 4);
        int m2 = Max2(1 * 2, 3 + 4);
        std::cout << m1 << ", " << m2 << std::endl;

        int i1 = 3, j1 = 5;
        int i2 = 3, j2 = 5;
        int m3 = Max1(i1++, j1++);
        int m4 = Max2(i2++, j2++);
        std::cout << m3 << ", " << m4 << std::endl;
    }

    // usage 1: use auto with basic types
    {
        int x = 123;
        static_assert(std::is_same_v<decltype(x), int>);
        int *y = &x;
        static_assert(std::is_same_v<decltype(y), int *>);
        int &z = x;
        static_assert(std::is_same_v<decltype(z), int &>);

        auto vx = x;
        static_assert(std::is_same_v<decltype(vx), int>);
        auto vy = y;
        static_assert(std::is_same_v<decltype(vy), int *>);
        auto vz = z;
        static_assert(std::is_same_v<decltype(vz), int>);

        auto *px = &x;
        static_assert(std::is_same_v<decltype(px), int *>);
        auto *py = y;
        static_assert(std::is_same_v<decltype(py), int *>);
        auto *pz = &z;
        static_assert(std::is_same_v<decltype(pz), int *>);

        auto &rx = x;
        static_assert(std::is_same_v<decltype(rx), int &>);
        auto &ry = y;
        static_assert(std::is_same_v<decltype(ry), int *&>);
        auto &rz = z;
        static_assert(std::is_same_v<decltype(rz), int &>);

        auto vf1 = func1();
        static_assert(std::is_same_v<decltype(vf1), double>);
        auto vf2 = func2();
        static_assert(std::is_same_v<decltype(vf2), float *>);
        auto vf3 = func3();
        static_assert(std::is_same_v<decltype(vf3), int>);

        // auto *pf1 = &func1();  // error
        auto *pf2 = func2();
        static_assert(std::is_same_v<decltype(pf2), float *>);
        auto *pf3 = &func3();
        static_assert(std::is_same_v<decltype(pf3), int *>);

        // auto &rf1 = func1();   // error
        auto &rf2 = *func2();
        static_assert(std::is_same_v<decltype(rf2), float &>);
        auto &rf3 = func3();
        static_assert(std::is_same_v<decltype(rf3), int &>);
    }

    // usage 2: use auto with const and volatile
    {
        auto const a = func1();
        static_assert(std::is_same_v<decltype(a), double const>);
        auto *const b1 = func2();
        static_assert(std::is_same_v<decltype(b1), float * const>);
        auto const *b2 = func2();
        static_assert(std::is_same_v<decltype(b2), float const *>);
        auto const &c = func3();
        static_assert(std::is_same_v<decltype(c), int const &>);

        auto va = a;
        static_assert(std::is_same_v<decltype(va), double>);
        auto pb1 = b1;
        static_assert(std::is_same_v<decltype(pb1), float *>);
        auto pb2 = b2;
        static_assert(std::is_same_v<decltype(pb2), float const *>);
        auto rc1 = c;
        static_assert(std::is_same_v<decltype(rc1), int>);
        auto &rc2 = c;
        static_assert(std::is_same_v<decltype(rc2), int const &>);
    }

    // usage 3: use auto with multiple variables in a sentence
    {
        auto x = 1, y = 2;
        static_assert(std::is_same_v<decltype(x), int>);
        static_assert(std::is_same_v<decltype(y), int>);
        auto const *m = &x, n = 1;
        static_assert(std::is_same_v<decltype(m), int const *>);
        static_assert(std::is_same_v<decltype(n), int const>);
        // auto i = 1, j = 3.14;  // error
    }

    // usage 4: some other examples
    {
        auto x(1);
        static_assert(std::is_same_v<decltype(x), int>);
        auto y { 1 };
        static_assert(std::is_same_v<decltype(y), int>);
        // auto z { 1, 2, 3 };  // error
        auto a1 = { 1, 2, 3 };
        static_assert(std::is_same_v<decltype(a1), std::initializer_list<int>>);
        auto a2 = { 1 };
        static_assert(std::is_same_v<decltype(a2), std::initializer_list<int>>);
        auto p = new auto(1);
        static_assert(std::is_same_v<decltype(p), int *>);
        delete p;
    }

    // some cases that auto doesn't work
    {
        // char x[3];
        // auto y[3] = x;                      // error
        // std::vector<auto> v = { 1, 2, 3 };  // error

        // auto ai = std::atomic<int>(12);     // error
        // auto ll = long long { 34 };         // error
    }
}

}
