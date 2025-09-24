#pragma once

#include <iostream>

namespace recipe_04_03
{

struct alignas(8) item
{
    int id;
    bool active;
    double value;
};

static_assert(sizeof(item) == 16, "size of item must be 16 bytes");

template <typename T>
class pod_wrapper
{
    static_assert(std::is_standard_layout_v<T> && std::is_trivial_v<T>,
            "POD type expected!");

    T value;
};

struct point
{
    int x;
    int y;
};

template <typename T>
auto mul(T const a, T const b)
{
    static_assert(std::is_integral_v<T>, "Integral type expected");

    return a * b;
}

void execute()
{
    // use static_assert in namespace scope
    {
        item it;
        std::cout << "sizeof(item) = " << sizeof(item) << std::endl;
        std::cout << std::hex << "&id = " << &(it.id) << ", &active = " <<
                &(it.active) << ", &value = " << &(it.value) << std::endl;
    }

    // use static_assert in class scope
    {
        pod_wrapper<int> pw1;             // OK
        pod_wrapper<point> pw2;           // OK
        // pod_wrapper<std::string> pw3;  // error: POD type expected
    }

    // use static_assert in function scope
    {
        auto v1 = mul(1, 2);           // OK
        // auto v2 = mul(12.0, 42.5);  // error: Integral type expected
    }
}

}
