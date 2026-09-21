#pragma once

#include "recipe_04_common.h"

namespace recipe_04_05
{

namespace enable_if_version
{
    template <typename T,
            typename = typename std::enable_if_t<std::is_pointer_v<T>, T>>
    auto value_of(T value)
    {
        return *value;
    }

    template <typename T,
            typename = typename std::enable_if_t<!std::is_pointer_v<T>, T>>
    T value_of(T value)
    {
        return value;
    }
}

namespace constexpr_if_version
{
    template <typename T>
    auto value_of(T value)
    {
        if constexpr (std::is_pointer_v<T>) {
            return *value;
        } else {
            return value;
        }
    }
}

template <typename T>
auto compute(T const a, T const b)
{
    if constexpr (std::is_integral_v<T>) {
        return a + b;
    } else {
        return a * b;
    }
}

namespace binary
{
    using byte8 = unsigned char;
    using byte16 = unsigned short;
    using byte32 = unsigned int;

    namespace binary_literals
    {
        namespace binary_literals_internals
        {
            template <typename CharT, char d, char ...bits>
            constexpr CharT binary_eval()
            {
                static_assert(d == '0' || d == '1',
                        "binary literal can only contain 0 or 1");

                if constexpr(sizeof...(bits) == 0) {
                    return static_cast<CharT>(d - '0');
                } else if constexpr(d == '0') {
                    return binary_eval<CharT, bits...>();
                } else {  // d == '1'
                    return static_cast<CharT>((1 << sizeof...(bits)) |
                            binary_eval<CharT, bits...>());
                }
            }
        }

        template <char ...bits>
        constexpr byte8 operator ""_b8()
        {
            static_assert(sizeof...(bits) <= 8,
                    "binary literal b8 must be up to 8 digits long");
            return binary_literals_internals::binary_eval<byte8, bits...>();
        }

        template <char ...bits>
        constexpr byte16 operator ""_b16()
        {
            static_assert(sizeof...(bits) <= 16,
                    "binary literal b16 must be up to 16 digits long");
            return binary_literals_internals::binary_eval<byte16, bits...>();
        }

        template <char ...bits>
        constexpr byte32 operator ""_b32()
        {
            static_assert(sizeof...(bits) <= 32,
                    "binary literal b32 must be up to 32 digits long");
            return binary_literals_internals::binary_eval<byte32, bits...>();
        }
    }
}

void execute()
{
    // value_of: enable_if_version
    {
        auto v1 = enable_if_version::value_of(42);
        auto p = std::make_unique<int>(42);
        auto v2 = enable_if_version::value_of(p.get());
        std::cout << v1 << ", " << v2 << std::endl;
    }

    // value_of: constexpr_if_version
    {
        auto v1 = constexpr_if_version::value_of(42);
        auto p = std::make_unique<int>(42);
        auto v2 = constexpr_if_version::value_of(p.get());
        std::cout << v1 << ", " << v2 << std::endl;
        std::cout << std::endl;
    }

    // binary literals: constexpr_if_version
    {
        using namespace binary;
        using namespace binary_literals;

        auto b1 = 1010_b8;
        std::cout << static_cast<unsigned int>(b1) << std::endl;
        auto b2 = 101010101010_b16;
        std::cout << b2 << std::endl;
        auto b3 = 010101010101010101010101_b32;
        std::cout << b3 << std::endl;
        // auto b4 = 101010101_b8;  // error
        // auto b5 = 1020_b8;       // error
    }
}

}
