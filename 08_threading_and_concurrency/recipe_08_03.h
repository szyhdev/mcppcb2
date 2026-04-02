#pragma once

#include "recipe_08_common.h"

namespace recipe_08_03
{

using namespace std::chrono_literals;

class foo_rec
{
    std::recursive_mutex rmx;
    int data;

public:
    foo_rec(int const d = 0) : data(d) {
    }

    void update(int const d) {
        std::lock_guard<std::recursive_mutex> lock(rmx);
        data = d;
    }

    void increment() {
        std::lock_guard<std::recursive_mutex> lock(rmx);
        data++;
    }

    int update_with_return(int const d) {
        std::lock_guard<std::recursive_mutex> lock(rmx);
        auto temp = data;
        update(d);
        return temp;
    }

    int increment_with_return() {
        std::lock_guard<std::recursive_mutex> lock(rmx);
        auto temp = data;
        increment();
        return temp;
    }

    int get_data() {
        std::lock_guard<std::recursive_mutex> lock(rmx);
        return data;
    }
};

void thread_func_recursive_mutex(foo_rec &fr)
{
    fr.increment_with_return();
}

void test_foo_rec()
{
    foo_rec fr;

    std::vector<std::thread> threads;
    for (int i = 0; i < 10000; ++i) {
        threads.emplace_back(thread_func_recursive_mutex, std::ref(fr));
    }

    for (auto &t : threads) {
        t.join();
    }

    std::cout << fr.get_data() << std::endl;
}

class foo
{
    std::mutex mx;
    int data;

private:
    int internal_update(int const d) {
        int temp = data;
        data = d;
        return temp;
    }

    int internal_increment() {
        return data++;
    }

public:
    foo(int const d = 0) : data(d) {
    }

    void update(int const d) {
        std::lock_guard<std::mutex> lock(mx);
        internal_update(d);
    }

    void increment() {
        std::lock_guard<std::mutex> lock(mx);
        internal_increment();
    }

    int update_with_return(int const d) {
        std::lock_guard<std::mutex> lock(mx);
        return internal_update(d);
    }

    int increment_with_return() {
        std::lock_guard<std::mutex> lock(mx);
        return internal_increment();
    }

    int get_data() {
        std::lock_guard<std::mutex> lock(mx);
        return data;
    }
};

void thread_func_mutex(foo &f)
{
    f.increment_with_return();
}

void test_foo()
{
    foo f;

    std::vector<std::thread> threads;
    for (int i = 0; i < 10000; ++i) {
        threads.emplace_back(thread_func_mutex, std::ref(f));
    }

    for (auto &t : threads) {
        t.join();
    }

    std::cout << f.get_data() << std::endl;
}

int counter_rec = 0;
std::recursive_mutex rmx;

void increment_counter_rec(int level)
{
    std::lock_guard<std::recursive_mutex> lg(rmx);
    if (level > 1) {

        increment_counter_rec(level - 1);
    }

    if (level > 0) {
        counter_rec++;
    }
}

void thread_func_increment_counter_rec(int level)
{
    increment_counter_rec(level);
}

void test_counter_rec()
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 100; ++i) {
        threads.emplace_back(thread_func_increment_counter_rec, 100);
    }

    for (auto &t : threads) {
        t.join();
    }

    std::cout << counter_rec << std::endl;
}

int counter = 0;
std::mutex mx;

void increment_counter(int level)
{
    if (level > 1) {
        increment_counter(level - 1);
    }

    if (level > 0) {
        counter++;
    }
}

void thread_func_increment_counter(int level)
{
    std::lock_guard<std::mutex> lg(mx);
    increment_counter(level);
}

void test_counter()
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 100; ++i) {
        threads.emplace_back(thread_func_increment_counter, 100);
    }

    for (auto &t : threads) {
        t.join();
    }

    std::cout << counter << std::endl;
}

void execute()
{
    // use recursive mutex
    auto tr = recipe_common::perf_timer<>::duration(test_foo_rec);
    std::cout << std::chrono::duration<double, std::micro>(tr).count() <<
            "us" << std::endl;
    std::cout << std::endl;

    // use non-recursive mutex
    auto tm = recipe_common::perf_timer<>::duration(test_foo);
    std::cout << std::chrono::duration<double, std::micro>(tm).count() <<
            "us" << std::endl;
    std::cout << std::endl;

    // use recursive mutex in recursive function
    tr = recipe_common::perf_timer<>::duration(test_counter_rec);
    std::cout << std::chrono::duration<double, std::micro>(tr).count() <<
            "us" << std::endl;
    std::cout << std::endl;

    // use non-recursive mutex in recursive function
    tm = recipe_common::perf_timer<>::duration(test_counter);
    std::cout << std::chrono::duration<double, std::micro>(tm).count() <<
            "us" << std::endl;
}

}
