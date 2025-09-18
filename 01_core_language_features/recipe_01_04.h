#pragma once

#include <iostream>

namespace recipe_01_04
{

enum class TextVerticalAligment
{
    Top, Middle, Bottom
};

enum class TextHorizontalAligment
{
    Left, Center, Right
};

struct Control
{
    // use default member initialization for constants
    const int DefaultHeight = 14;
    const int DefaultWidth = 80;

    // use default member initialization to provide default values for members
    // of classes with multiple constructors
    TextVerticalAligment valign = TextVerticalAligment::Middle;
    TextHorizontalAligment halign = TextHorizontalAligment::Left;

    std::string text;

    // use the constructor initializer list to initialize members that don't have
    // default values, but depend on constructor parameters
    Control(std::string const &t) : text(t) {
    }

    Control(std::string const &t,
            TextVerticalAligment const va,
            TextHorizontalAligment const ha) : text(t), valign(va), halign(ha) {
    }
};

struct Point
{
    double X;
    double Y;

    Point(double const x = 0.0, double const y = 0.0) : X(x), Y(y) {
    }
};

struct foo
{
    foo() {
        std::cout << "default constructor" << std::endl;
    }

    foo(std::string const &text) {
        std::cout << "constructor '" << text << "'" << std::endl;
    }

    foo(foo const &other) {
        std::cout << "copy constructor" << std::endl;
    }

    foo(foo &&other) {
        std::cout << "move constructor" << std::endl;
    };

    foo &operator =(foo const &other) {
        std::cout << "copy assignment" << std::endl;
        return *this;
    }

    foo &operator =(foo &&other) {
        std::cout << "move assignment" << std::endl;
        return *this;
    }

    ~foo() {
        std::cout << "destructor" << std::endl;
    }
};

struct bar_wrong
{
    foo f;

    bar_wrong(foo const &value) {
        f = value;
    }
};

struct bar_right
{
    foo f;

    bar_right(foo const &value) : f(value) {
    }
};

struct bar_new
{
    foo f { "default value" };

    bar_new() : f { "constructor initializer" } {
    }
};

void execute()
{
    // initialize non-static members of a class
    {
        Control ctrl("example", TextVerticalAligment::Top,
                TextHorizontalAligment::Center);
    }

    // non-static data members initialized in the constructor's initializer list
    {
        Point pt(100, 100);
    }

    // using assignments in the constructor is not efficient
    {
        foo f;            // default constructor: f
        bar_wrong bw(f);  // default constructor: bw.f
                          // copy assignment: f = value
        // destructor: bw.f
        // destructor: f
    }

    // using copy constructor in initializer list is efficient
    {
        foo f;            // default constructor: f
        bar_right br(f);  // copy constructor: br.f
        // destructor: br.f
        // destructor: f
    }

    // constructor initializer list take precedence over default member initialization
    {
        bar_new bn;  // parameterized constructor: b.f
        // destructor: bn.f
    }
}

}
