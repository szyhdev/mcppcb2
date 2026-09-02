#pragma once

#include "recipe_02_common.h"

namespace recipe_02_06
{

#define USE_RECURSIVE_TEMPLATE
#define USE_FOLD_EXPRESSION

namespace binary
{
    using byte8 = unsigned char;
    using byte16 = unsigned short;
    using byte32 = unsigned int;

    namespace binary_literals
    {
        namespace binary_literals_internals
        {
        #if defined(USE_RECURSIVE_TEMPLATE)
            template <typename T, char... bits>
            struct binary_struct;

            template <typename T, char... bits>
            struct binary_struct<T, '0', bits...>
            {
                static constexpr T value {
                        binary_struct<T, bits...>::value
                };
            };

            template <typename T, char... bits>
            struct binary_struct<T, '1', bits...>
            {
                static constexpr T value {
                    static_cast<T>(1 << sizeof...(bits)) |
                            binary_struct<T, bits...>::value
                };
            };

            template <typename T>
            struct binary_struct<T>
            {
                static constexpr T value { 0 };
            };
        #endif

        #if defined(USE_FOLD_EXPRESSION)
            template <typename T, char... bits>
            struct binary_struct
            {
                static constexpr T value = [] () constexpr {
                    T result = 0;
                    ((result = (result << 1) | (bits == '1' ? 1 : 0)), ...);
                    return result;
                }();
            };
        #endif
        }

        template <char... bits>
        constexpr byte8 operator ""_b8()
        {
            static_assert(sizeof...(bits) <= 8,
                    "binary literal b8 must be up to 8 digits long");
            return binary_literals_internals::binary_struct<byte8, bits...>::value;
        }

        template <char... bits>
        constexpr byte16 operator ""_b16()
        {
            static_assert(sizeof...(bits) <= 16,
                    "binary literal b16 must be up to 16 digits long");
            return binary_literals_internals::binary_struct<byte16, bits...>::value;
        }

        template <char... bits>
        constexpr byte32 operator ""_b32()
        {
            static_assert(sizeof...(bits) <= 32,
                    "binary literal b32 must be up to 32 digits long");
            return binary_literals_internals::binary_struct<byte32, bits...>::value;
        }
    }
}

void execute()
{
    using namespace binary;
    using namespace binary_literals;

    auto b1 = 1010_b8;
    std::cout << (uint)b1 << std::endl;
    auto b2 = 101010101010_b16;
    std::cout << (uint)b2 << std::endl;
    auto b3 = 10101010101010101010101_b32;
    std::cout << (uint)b3 << std::endl;

    // error: binary literal b8 must be up to 8 digits long
    // auto b4 = 0011111111_b8;

    // error: binary literal b16 must be up to 16 digits long
    // auto b5 = 001111111111111111_b16;

    // error: binary literal b32 must be up to 32 digits long
    // auto b6 = 0011111111111111111111111111111111_b32;
}

}
