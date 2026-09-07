#pragma once

#include "recipe_04_common.h"

namespace recipe_04_01
{

#if !defined(UNIQUE_NAME)
#define UNIQUE_NAME
class widget
{
};
#endif

void show_compiler()
{
#if defined _MSC_VER
    std::cout << "Visual C++" << std::endl;
#elif defined __clang__
    std::cout << "Clang" << std::endl;
#elif defined __GNUG__
    std::cout << "GCC" << std::endl;
#else
    std::cout << "Unknown compiler" << std::endl;
#endif
}

void show_compiler_version()
{
#if defined _MSC_VER

#if (_MSC_VER >= 1900)
    std::cout << "Visual C++ 2019 or newer" << std::endl;
#else
    std::cout << "Visual C++ " << _MSC_FULL_VER << std::endl;
#endif

#elif defined __clang__

#if (__clang_major__ == 3) && (__clang_minor__ >= 9)
    std::cout << "Clang 3.9 or newer" << std::endl;
#else
    std::cout << "Clang " << __clang_version__ << std::endl;
#endif

#elif defined __GNUG__

#if __GNUC__ >= 5 && __GNUC_MINOR__ > 0
    std::cout << "GCC 5.0 or newer" << std::endl;
#else
    std::cout << "GCC " << __VERSION__ << std::endl;
#endif

#else
    std::cout << "Unknown compiler" << std::endl;
#endif
}

void show_architecture()
{
#if defined _MSC_VER

#if defined _M_X64
    std::cout << "AMD64" << std::endl;
#elif defined _M_IX86
    std::cout << "INTEL x86" << std::endl;
#elif defined _M_ARM
    std::cout << "ARM" << std::endl;
#else
    std::cout << "unknown" << std::endl;
#endif

#elif defined __clang__ || __GNUG__

#if defined __amd64__
    std::cout << "AMD64" << std::endl;
#elif defined __i386__
    std::cout << "INTEL x86" << std::endl;
#elif defined __arm__
    std::cout << "ARM" << std::endl;
#else
    std::cout << "unknown" << std::endl;
#endif

#else
#error Unknown compiler
#endif
}

void show_configuration()
{
#ifdef _DEBUG
    std::cout << "debug" << std::endl;
#else
    std::cout << "release" << std::endl;
#endif
}

void execute()
{
    // print name of compiler
    show_compiler();

    // print version of compiler
    show_compiler_version();

    // print name of architecture
    show_architecture();

    // print name of configuration
    show_configuration();
}

}
