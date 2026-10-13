#pragma once

#include "recipe_01_common.h"

namespace recipe_01_03
{

class foo
{
    int x_;

public:
    foo(int x) : x_(x) {
    }
};

struct bar
{
    int a_;
    double b_;

public:
    bar() : a_(0), b_(0.0) {
    }

    bar(int a, double b = 0.0) : a_(a), b_(b) {
    }

    void print() const {
        std::cout << a_ << ", " << b_ << std::endl;
    }
};

struct pod_type
{
    int a_;
    double b_;
};

struct baz
{
    int a_;
    int b_;

public:
    baz() : a_(0), b_(0) {
        std::cout << "baz(): " << a_ << ", " << b_ << std::endl;
    }

    baz(int a, int b = 0) : a_(a), b_(b) {
        std::cout << "baz(int, int): " << a_ << ", " << b_ << std::endl;
    }

    baz(std::initializer_list<int> list) {
        std::cout << "baz(std::initializer_list<int>): ";
        for (auto const &e : list) {
            std::cout << e << " ";
        }
        std::cout << std::endl;
    }
};

struct three_int
{
    int a;
    int b;
    int c;
};

void func(three_int const &ti)
{
    std::cout << "func(three_int const &): " <<
            ti.a << " " << ti.b << " " << ti.c << std::endl;
}

void func(int const (&ar)[3])
{
    std::cout << "func(int const (&)[3]): " <<
            ar[0] << " " << ar[1] << " " << ar[2] << std::endl;
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
    // different types of initialization before C++11
    {
        int i = 42;
        double d = 1.2;
        bool c = false;

        foo f = 42;
        bar b = 42;

        bar b1;       // default initialization
        bar b2(42, 1.2);
        bar b3(42);
        // bar b4();  // error: function declaration
        static_assert(!std::is_aggregate_v<bar>);

        three_int ti = { 1, 2, 3 };
        int ar[] = { 1, 2, 3, 4, 5 };

        // std::list<int> l1 { 1, 2, 3 };  // error
        std::list<int> l2(ar, ar + 5);
        std::list<int> l3(l2);
    }

    // direct initialization and copy initialization vs.
    // direct list initialization and copy list initialization
    {
        std::string s11("test");
        std::string s12 { "test" };

        std::string s21 = "test";
        std::string s22 = { "test" };
    }

    // examples of uniform initialization
    {
        // built-in types
        int i { 42 };
        double d { 1.2 };
        bool c { false };

        // user-defined non-aggregate type
        bar b1;
        b1.print();
        bar b2 {};
        b2.print();
        bar b3 { 42, 1.2 };
        b3.print();
        bar b4 { 42 };
        b4.print();

        // user-defined aggregate type
        three_int ti1;
        std::cout << ti1.a << ", " << ti1.b << ", " << ti1.c << std::endl;
        three_int ti2 {};
        std::cout << ti2.a << ", " << ti2.b << ", " << ti2.c << std::endl;
        three_int ti3 { 1, 2, 3 };
        std::cout << ti3.a << ", " << ti3.b << ", " << ti3.c << std::endl;
        three_int ti4 { 1, 2 };
        std::cout << ti4.a << ", " << ti4.b << ", " << ti4.c << std::endl;
        std::cout << std::endl;

        // pod type
        pod_type pt { 42, 1.2 };
        static_assert(std::is_standard_layout_v<pod_type> &&
                std::is_trivial_v<pod_type>);
        static_assert(std::is_aggregate_v<pod_type>);

        // array and dynamic array
        int ar1[] { 1, 2, 3, 4, 5 };
        int ar2[3] { 1, 2, 3 };
        int *ar = new int[3] { 1, 2, 3 };
        delete [] ar;

        // standard containers
        std::vector<int> v { 1, 2, 3 };
        std::map<int, std::string> m {
            { 1, "one" },
            { 2, "two" },
            { 3, "three" }
        };
    }

    // an initializer list takes precedence over other constructors or
    // functions where brace-initialization is used
    {
        baz b { 1, 2 };
        func({ 1, 2, 3 });
    }

    // initialization with '{}' or '()'
    {
        std::vector<int> v1 { 5 };
        assert(v1.size() == 1);
        std::vector<int> v2(5);
        assert(v2.size() == 5);
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

    // examples of direct-list-initialization and copy-list-initialization
    {
        // in C++11
        auto a1 = { 42 };     // std::initializer_list<int>
        auto b1 { 42 };       // std::initializer_list<int>
        auto c1 = { 4, 2 };   // std::initializer_list<int>
        // auto d1 { 4, 2 };  // std::initializer_list<int>

        // in C++17 and above
        auto a2 = { 42 };     // std::initializer_list<int>
        auto b2 { 42 };       // int
        auto c2 = { 4, 2 };   // std::initializer_list<int>
        // auto d2 { 4, 2 };  // error: only one element required
    }
}

}
