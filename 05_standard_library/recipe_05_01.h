#pragma once

#include "recipe_05_common.h"

namespace recipe_05_01
{

struct foo
{
    int i;
    double d;
    std::string s;

    foo (int i, double d, std::string const &s) : i(i), d(d), s(s) {
    }
};

std::ostream &operator <<(std::ostream &os, foo const &f)
{
    os << "(" << f.i << ", " << f.d << ", " << f.s << ")";
    return os;
}

void execute()
{
    // initialize std::vector
    {
        // initialize from an initialization list
        std::vector<int> v1 { 1, 2, 3, 4, 5 };
        recipe_common::print_collection(v1, "v1: ");

        // initialize from an array
        int arr[] = { 1, 2, 3, 4, 5 };
        std::vector<int> v21(arr, arr + 5);
        recipe_common::print_collection(v21, "v21: ");
        std::vector<int> v22(arr + 1, arr + 4);
        recipe_common::print_collection(v22, "v22: ");

        // initialize from another container
        std::list<int> l { 1, 2, 3, 4, 5 };
        std::vector<int> v3(l.begin(), l.end());
        recipe_common::print_collection(v3, "v3: ");

        // initialize from a count and a value
        std::vector<int> v4(5, 1);
        recipe_common::print_collection(v4, "v4: ");
        std::cout << std::endl;
    }

    // modify content of std::vector
    {
        // push_back and pop_back
        std::vector<int> v1 { 1, 2, 3, 4, 5 };
        v1.push_back(6);
        recipe_common::print_collection(v1, "v1: ");
        v1.pop_back();
        recipe_common::print_collection(v1, "v1: ");

        // insert
        int arr[] = { 1, 2, 3, 4, 5 };
        std::vector<int> v21;
        v21.insert(v21.begin(), arr, arr + 5);
        recipe_common::print_collection(v21, "v21: ");

        std::vector<int> v22;
        v22.insert(v22.begin(), arr + 1, arr + 4);
        recipe_common::print_collection(v22, "v22: ");

        // emplace_back and emplace
        std::vector<foo> v3;
        v3.emplace_back(1, 1.0, "one");
        recipe_common::print_collection(v3, "v3: ");
        v3.emplace(v3.begin(), 2, 2.0, "two");
        recipe_common::print_collection(v3, "v3: ");
        std::cout << std::endl;
    }

    // modify whole content of std::vector
    {
        // assignment
        std::vector<int> v1 { 1, 2, 3, 4, 5 };
        recipe_common::print_collection(v1, "v1: ");
        std::vector<int> v2 { 10, 20, 30 };
        recipe_common::print_collection(v2, "v2: ");
        v2 = v1;
        recipe_common::print_collection(v2, "v2: ");

        // assign
        int arr[] = { 1, 2, 3, 4, 5 };
        std::vector<int> v31;
        v31.assign(arr, arr + 5);
        recipe_common::print_collection(v31, "v31: ");
        std::vector<int> v32;
        v32.assign(arr + 1, arr + 4);
        recipe_common::print_collection(v32, "v32: ");

        // swap
        std::vector<int> v4 { 1, 2, 3, 4, 5 };
        recipe_common::print_collection(v4, "v4: ");
        std::vector<int> v5 { 10, 20, 30 };
        recipe_common::print_collection(v5, "v5: ");
        v4.swap(v5);
        recipe_common::print_collection(v4, "v4: ");
        recipe_common::print_collection(v5, "v5: ");

        // clear
        std::vector<int> v6 { 1, 2, 3, 4, 5 };
        recipe_common::print_collection(v6, "v6: ");
        v6.clear();
        recipe_common::print_collection(v6, "v6: ");

        // erase
        std::vector<int> v7 { 1, 2, 3, 4, 5 };
        recipe_common::print_collection(v7, "v7: ");
        v7.erase(v7.begin() + 1, v7.begin() + 4);
        recipe_common::print_collection(v7, "v7: ");
        std::cout << std::endl;
    }

    // get address of first element in a vector
    {
        std::vector<int> v { 1, 2, 3, 4, 5 };
        recipe_common::print_array(v.data(), v.size(), "v: ");
        recipe_common::print_array(&v[0], v.size(), "v: ");
        recipe_common::print_array(&v.front(), v.size(), "v: ");
        recipe_common::print_array(&(*v.begin()), v.size(), "v: ");
        std::cout << std::endl;
    }

    // swap: free additional reserved memory
    {
        std::vector<int> v1 { 1, 2, 3, 4, 5 };
        v1.reserve(10);
        std::cout << v1.size() << ", " << v1.capacity() << std::endl;
        v1.shrink_to_fit();
        std::cout << v1.size() << ", " << v1.capacity() << std::endl;
        recipe_common::print_array(v1.data(), v1.size(), "v1: ");

        std::vector<int> v2 { 1, 2, 3, 4, 5 };
        std::cout << v2.size() << ", " << v2.capacity() << std::endl;
        std::vector<int>().swap(v2);
        std::cout << v2.size() << ", " << v2.capacity() << std::endl;
        recipe_common::print_array(v2.data(), v2.size(), "v2: ");
    }
}

}
