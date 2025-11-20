#pragma once

#include "recipe_09_common.h"

#include <map>
#include <mutex>
#include <vector>

namespace recipe_09_03
{

struct session
{
};

session connect(std::string const &uri, int const timeout = 2000)
{
    // do something
    return session {};
}

class user_settings
{
    bool show_online;

public:
    bool can_show_online() const {
        return show_online;
    }

    int const min_update_interval = 15;
};

user_settings get_user_settings()
{
    return user_settings {};
}

void update()
{
    user_settings const us = get_user_settings();

    if (us.can_show_online()) {
        // do something
    }

    std::cout << std::boolalpha << "user_settings: show_online = " <<
            us.can_show_online() << std::endl;
}

class computation
{
    double compute_value(double const input) const {
        return input;
    }

    mutable std::map<double, double> cache;

public:
    double compute(double const input) const {
        auto it = cache.find(input);
        if (it != cache.end()) {
            return it->second;
        }

        auto result = compute_value(input);
        cache[input] = result;

        return result;
    }
};

template <typename T>
class container
{
    std::vector<T> data;
    mutable std::mutex mt;

public:
    void add(T const &value) {
        std::lock_guard<std::mutex> lock(mt);
        data.push_back(value);
    }

    bool contains(T const &value) const {
        std::lock_guard<std::mutex> lock(mt);
        return std::find(std::begin(data), std::end(data), value) != std::end(data);
    }
};

class contact
{
public:
    std::string name;
    unsigned int age;
};

class addressbook
{
    std::vector<contact> contacts;

public:
    void add(contact const &c) {
        contacts.push_back(c);
    }

    contact &operator [](size_t const index) {
        std::cout << "non-constant version of []: ";
        return contacts[index];
    }

    contact const &operator [](size_t const index) const {
        std::cout << "constant version of []: ";
        return contacts[index];
    }
};

void execute()
{
    // declare the following as constants
    {
        // parameters to functions that are not supposed to be modified
        session se = connect("https:www.baidu.com", 1000);

        // class data members that do not change
        // class member functions that do not modify the object state
        user_settings us = get_user_settings();

        // function locals whose value do not change
        update();
        std::cout << std::endl;
    }

    // use const keyword with pointers
    {
        int i = 12;
        int j = 24;
        int const ic1 = 4;
        int const ic2 = 8;

        int *pi = &i;
        // int *pi = &ic1;      // error
        int const *pic = &ic1;
        pic = &ic2;
        // *pic = 12;           // error

        int const * const picc = &ic1;
        // picc = &ci2;         // error
        // *picc = 12;          // error

        int * const cpi = &i;
        // cpi = &j;  // error
        *cpi = 13;

        // int const **p = &pi;  // error
        int const **p = &pic;
        *p = &ic1;
    }

    // data members that are supposed to be modified from constant methods
    // should be declared mutable
    {
        computation const c;
        c.compute(42);
        std::cout << "mutable cache" << std::endl;
    }

    // in order to lock and unlock the mutex, both mutable operations and
    // the mutex must be declared mutable
    {
        container<int> c;
        c.add(42);
        auto exists = c.contains(42);
        std::cout << "mutable mutex" << std::endl;
        std::cout << std::endl;
    }

    // a method or an operator overloaded to have both constant and
    // non-constant versions
    {
        addressbook ab;
        ab.add({ "john", 12 });
        ab.add({ "tom", 13 });
        ab.add({ "william", 14 });

        ab[1].age *= 2;
        std::cout << std::endl;
        std::cout << ab[1].age << std::endl;
        std::cout << std::endl;

        addressbook const &rab = ab;
        // rab[1].age /= 2;  // error
        ab[1].age /= 2;
        std::cout << std::endl;
        std::cout << rab[1].age << std::endl;
    }
}

}
