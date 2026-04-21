#pragma once

#include "recipe_03_common.h"

namespace recipe_03_09_common
{

int add(int a, int b)
{
    return a + b;
}

int sub(int a, int b)
{
    return a - b;
}

int calc(int (*fp)(int, int), int a, int b)
{
    return fp(a, b);
}

class IntAdd
{
public:
    int operator ()(int a, int b) {
        return a + b;
    }
};

struct IntCompare
{
    typedef int first_argument_type;
    typedef int second_argument_type;
    typedef bool result_type;

    bool operator ()(int x, int y) const {
        return x > y;
    }
};

struct IsOdd
{
    typedef int argument_type;
    typedef bool result_type;

    bool operator ()(int x) const {
        return x % 2 == 1;
    }
};

bool is_odd(int x)
{
    return x % 2 == 1;
}

struct foo
{
    int x = 0;

    void increment(int n) {
        x += n;
    }
};

class Student
{
private:
    std::string name;
    int score;

public:
    Student(std::string const &n, int s) : name(n), score(s) {
    }

    void Display() const {
        std::cout << name << ": " << score << std::endl;
    }

    int AddScore(int bonus) {
        score += bonus;
        return score;
    }
};

}
