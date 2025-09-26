#pragma once

#include "../include/recipe_common.h"

#include <iostream>

namespace recipe_05_common
{

struct Task
{
    int priority;
    std::string name;
};

bool operator <(Task const &lhs, Task const &rhs) {
    return lhs.priority < rhs.priority;
}

bool operator >(Task const &lhs, Task const &rhs) {
    return lhs.priority > rhs.priority;
}

std::ostream &operator <<(std::ostream &os, Task const &task)
{
    os << "(" << task.priority << ", " << task.name << ") ";

    return os;
}

}
