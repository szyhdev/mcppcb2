#pragma once

#include "recipe_05_common.h"

namespace recipe_05_09
{

template <typename C>
class back_insert_iterator
{
public:
    typedef back_insert_iterator<C> T;
    typedef typename C::value_type V;

    explicit back_insert_iterator(C &c) : container(&c) {
    }

    T &operator =(const V &val) {
        container->push_back(val);
        return *this;
    }

    T &operator *() {
        return *this;
    }

    T &operator ++() {
        return *this;
    }

    T &operator ++(int) {
        return *this;
    }

protected:
    C *container;
};

void execute()
{
    // back_inserter
    {
        std::vector<int> v { 1, 2, 3, 4, 5 };
        std::fill_n(std::back_inserter(v), 3, 0);
        recipe_common::print_collection(v, "v: ");
        std::cout << std::endl;
    }

    // front_inserter
    {
        std::list<int> l { 1, 2, 3, 4, 5 };
        std::fill_n(std::front_inserter(l), 3, 0);
        recipe_common::print_collection(l, "l: ");
        std::cout << std::endl;
    }

    // inserter with random iterator of vector
    {
        std::vector<int> v { 1, 2, 3, 4, 5 };
        std::fill_n(std::inserter(v, v.begin() + 3), 3, 0);
        recipe_common::print_collection(v, "v: ");
        std::cout << std::endl;
    }

    // inserter with bidirectional iterator of list
    {
        std::list<int> l { 1, 2, 3, 4, 5 };
        auto itr = l.begin();
        std::advance(itr, 3);
        std::fill_n(std::inserter(l, itr), 3, 0);
        recipe_common::print_collection(l, "l: ");
        std::cout << std::endl;
    }

    // use inserter to insert single element (not recommended)
    {
        std::vector<int> v { 1, 2, 3, 4, 5 };
        std::back_insert_iterator<std::vector<int>> v_itr(v);
        *v_itr = 6;
        recipe_common::print_collection(v, "v: ");

        std::list<int> l { 1, 2, 3, 4, 5 };
        std::front_insert_iterator<std::list<int>> l_itr(l);
        *l_itr = 0;
        recipe_common::print_collection(l, "l: ");
    }
}

}
