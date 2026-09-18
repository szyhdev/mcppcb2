#pragma once

#include "recipe_04_common.h"

namespace recipe_04_03
{

struct alignas(8) item
{
    int id;
    bool active;
    double value;
};

static_assert(sizeof(item) == 16, "size of item must be 16 bytes");

struct point
{
    int x;
    int y;
};

template <typename T>
class pod_wrapper
{
    static_assert(std::is_standard_layout_v<T> && std::is_trivial_v<T>,
            "POD type expected");
    T value;
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

        std::uintptr_t id_addr = reinterpret_cast<std::uintptr_t>(&it.id);
        assert(id_addr % 8 == 0);
        std::uintptr_t active_addr = reinterpret_cast<std::uintptr_t>(&it.active);
        assert(active_addr % 8 == 4);
        std::uintptr_t value_addr = reinterpret_cast<std::uintptr_t>(&it.value);
        assert(value_addr % 8 == 0);

        std::cout << "offset of id: " << offsetof(item, id) << std::endl;
        std::cout << "offset of active: " << offsetof(item, active) << std::endl;
        std::cout << "offset of value: " << offsetof(item, value) << std::endl;
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
