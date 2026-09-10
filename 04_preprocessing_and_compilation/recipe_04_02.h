#pragma once

#include "recipe_04_common.h"

namespace recipe_04_02
{

#define NUMBER 42

#define MAKE_STR2(x) #x
#define MAKE_STR(x)  MAKE_STR2(x)

#define HELLO Hello
#define WORLD World

#define MERGE2(x, y) x##y
#define MERGE(x, y)  MERGE2(x, y)

#define DECL_MAKE(x)  DECL_MAKE2(x)
#define DECL_MAKE2(x) x *make##_##x() { return new x(); }

struct bar {};
struct foo {};

DECL_MAKE(foo)
DECL_MAKE(bar)

void execute()
{
    // stringizing operator: use text as argument
    {
        std::string s1 { MAKE_STR2(sample) };
        std::cout << s1 << std::endl;
        std::string s2 { MAKE_STR(sample) };
        std::cout << s2 << std::endl;
        std::cout << std::endl;
    }

    // stringizing operator: use macro as argument
    {
        std::string s3 { MAKE_STR2(NUMBER) };
        std::cout << s3 << std::endl;
        std::string s4 { MAKE_STR(NUMBER) };
        std::cout << s4 << std::endl;
        std::cout << std::endl;
    }

    // token-pasting operator: use text or macro as argument
    {
        // std::string s5 { MERGE2(Hello, World) };  // compile error
        // std::cout << s5 << std::endl;
        // std::string s6 { MERGE(Hello, World) };   // compile error
        // std::cout << s6 << std::endl;
        std::string s7 { MAKE_STR(MERGE2(Hello, World)) };
        std::cout << s7 << std::endl;
        std::string s8 { MAKE_STR(MERGE(Hello, World)) };
        std::cout << s8 << std::endl;
        std::string s9 { MAKE_STR(MERGE2(HELLO, WORLD)) };
        std::cout << s9 << std::endl;
        std::string s10 { MAKE_STR(MERGE(HELLO, WORLD)) };
        std::cout << s10 << std::endl;
    }

    // use token-pasting operator to provide factory methods
    {
        auto f = make_foo();
        static_assert(std::is_same_v<decltype(f), foo *>);
        auto b = make_bar();
        static_assert(std::is_same_v<decltype(b), bar *>);
    }
}

}
