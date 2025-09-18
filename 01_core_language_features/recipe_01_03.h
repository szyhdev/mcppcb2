#pragma once

#include <iostream>
#include <map>
#include <vector>

namespace recipe_01_03
{

class foo_int
{
    int a_;

public:
    foo_int(int a) :a_(a) {
    }
};

class foo_int_double
{
    int a_;
    double b_;

public:
    foo_int_double() :a_(0), b_(0) {
    }

    foo_int_double(int a, double b = 0.0) : a_(a), b_(b) {
    }
};

class foo_int_int
{
    int a_;
    int b_;

public:
    foo_int_int() : a_(0), b_(0) {
        std::cout << "foo_int_int(): " << a_ << " " << b_ << std::endl;
    }

    foo_int_int(int a, int b = 0) : a_(a), b_(b) {
        std::cout << "foo_int_int(int, int): " << a_ << " " << b_ << std::endl;
    }

    foo_int_int(std::initializer_list<int> list) {
        std::cout << "foo_int_int(std::initializer_list<int>): ";
        for (auto const &e : list) {
            std::cout << e << " ";
        }
        std::cout << std::endl;
    }
};

struct bar
{
    int a_;
    double b_;
};

void func(int const a, int const b, int const c)
{
    std::cout << "func(int const, int const, int const): " <<
            a << " " << b << " " << c << std::endl;
}

void func(std::initializer_list<int> const list)
{
    std::cout << "func(std::initializer_list<int> const): ";
    for (auto const &e : list) {
        std::cout << e << " ";
    }
    std::cout << std::endl;
}

void execute()
{
    // use the uniform initialization form '{}' to uniformly initialize objects
    {
        std::string s1("test");       // direct initialization
        std::string s2 { "test" };

        std::string s3 = "test";      // copy initialization
        std::string s4 = { "test" };
    }

    // examples of uniform initialization
    {
        // standard containers
        std::vector<int> v { 1, 2, 3 };
        std::map<int, std::string> m {
            { 1, "one" },
            { 2, "two" },
            { 3, "three" }
        };

        // arrays
        int arr1[] { 1, 2, 3, 4, 5 };
        int arr2[3] { 1, 2, 3 };
        int *arr3 = new int[3] { 1, 2, 3 };
        delete [] arr3;

        // built-in types
        int i { 42 };
        double d { 1.2 };

        // user-defined types
        foo_int_double fid1;
        foo_int_double fid2 { 42, 1.2 };
        foo_int_double fid3 { 42 };

        // user-defined POD types
        bar b { 42, 1.2 };
    }

    // different types of initialization based on their type before C++11
    {
        // fundamental types initialized using assignment
        int a = 42;
        double d = 1.2;
        bool c = false;

        // class objects initialized using assignment
        foo_int fi1 = 42;

        // non-aggregate classes initialized with parentheses
        foo_int_double fid1;
        foo_int_double fid2(42, 1.2);
        foo_int_double fid3(42);
        // foo_int_double fid4();  // warning: disambiguated as function declaration

        // aggregate and POD types initialized with brace-initialization
        bar b = { 42, 1.2 };
        int arr[] = {1, 2, 3, 4, 5};
    }

    // an initializer list takes precedence over other constructors or functions
    {
        foo_int_int f { 1, 2 };

        func({ 1, 2, 3 });
    }

    // initialization with the form '{}' or '()'
    {
        std::vector<int> v1 { 5 };
        std::cout << "v1.size() = " << v1.size() << std::endl;
        std::vector<int> v2(5);
        std::cout << "v2.size() = " << v2.size() << std::endl;
    }

    // brace-initialization doesn't allow narrowing conversion
    {
        // int i { 1.2 };  	                 // error
        int i { static_cast<int>(1.2) };     // OK

        double d = 47 / 13;
        // float f1 { d };                   // error
        float f1 { static_cast<float>(d) };  // OK
        float f2 { 47 / 13 };                // OK
    }

    // the type that matches brace-initialization cannot be deduced
    {
        // in C++11
        auto a1 = { 42 };     // std::initializer_list<int>
        auto b1 { 42 };       // std::initializer_list<int>
        auto c1 = { 4, 2 };   // std::initializer_list<int>
        // auto d1 { 4, 2 };  // std::initializer_list<int>

        // in C++14
        auto a2 = { 42 };     // std::initializer_list<int>
        auto b2 { 42 };       // int
        auto c2 = { 4, 2 };   // std::initializer_list<int>
        // auto d2 { 4, 2 };  // error: only one element required
    }
}

}
