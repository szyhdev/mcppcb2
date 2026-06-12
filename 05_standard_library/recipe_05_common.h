#pragma once

#include "../include/recipe_common.h"

namespace recipe_05_common
{

struct task
{
    int priority;
    std::string name;
};

bool operator <(task const &lhs, task const &rhs) {
    return lhs.priority < rhs.priority;
}

bool operator >(task const &lhs, task const &rhs) {
    return lhs.priority > rhs.priority;
}

std::ostream &operator <<(std::ostream &os, task const &t)
{
    os << "(" << t.priority << ", " << t.name << ") ";
    return os;
}

}
