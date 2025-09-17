#pragma once

#include <iostream>
#include <vector>

namespace recipe_01_02
{

void func(unsigned char b, double d)
{
    std::cout << "void func(unsigned char, double) is called" << std::endl;
}

template <typename T>
class foo_using
{
public:
    using value_type = T;
};

template <class T>
class custom_allocator
{
    /* ... */
};

template <typename T>
using vec_t_alloc = std::vector<T, custom_allocator<T>>;

template <typename T>
using vec_t = std::vector<T>;

typedef std::vector<int> vint_t;  // OK

// template <typename T>
// typedef std::vector<T> vec_t;  // error

template <typename T>
class foo_typedef
{
public:
    typedef T value_type;
};

void execute()
{
    // create type aliases with the form 'using identifier = type-id'
    {
        using byte = unsigned char;
        using byte_ptr = unsigned char *;
        using array_t = int[10];
        using fn = void(byte, double);
        using bar = struct {
            int value;
        };
        using pbar = bar *;

        byte by { 42 };
        byte_ptr pby = new byte[10] { 0 };
        delete [] pby;
        array_t a { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        fn *f = func;
        f(1, 42.0);
        bar b;
        b.value = 100;
        pbar pb = &b;
        pb->value = 101;
    }

    // create alias templates with the form 'template <template-params-list> identifier = type-id'
    {
        std::cout << typeid(foo_using<int>::value_type).name() << std::endl;
        vec_t<int> vi { 1, 2, 3 };
        vec_t<std::string> vs { "first", "second", "third" };
    }

    // create an alias for a type with the form 'typedef existing_type new_type'
    {
        typedef unsigned char byte;
        typedef unsigned char *byte_ptr;
        typedef int array_t[10];
        typedef void(*fn)(byte, double);
        typedef struct {
            int value;
        } bar, *pbar;

        byte by { 42 };
        byte_ptr pby = new byte[10] { 0 };
        delete [] pby;
        array_t a { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        fn f = func;
        f(1, 42.0);
        bar b;
        b.value = 100;
        pbar pb = &b;
        pb->value = 101;

        std::cout << typeid(foo_typedef<int>::value_type).name() << std::endl;
    }
}

}
