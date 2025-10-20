#pragma once

#include "recipe_08_common.h"

#include <mutex>
#include <thread>

namespace recipe_08_03
{

using namespace std::chrono_literals;

class foo_recursive
{
    std::recursive_mutex mx;
    int data;

public:
    foo_recursive(int const d = 0) : data(d) {
    }

    // void update(int const d) {
    void increment() {
        std::lock_guard<std::recursive_mutex> lock(mx);
        // data = d;
        data++;
    }

    // int update_with_return(int const d) {
    int increment_with_return() {
        std::lock_guard<std::recursive_mutex> lock(mx);
        auto temp = data;
        // update(d);
        increment();
        return temp;
    }

    int get_data() {
        std::lock_guard<std::recursive_mutex> lock(mx);
        return data;
    }

};

class foo
{
    std::mutex mx;
    int data;

private:
    // void internal_update(int const d) {
    void internal_increment() {
        // data = d;
        data++;
    }

    int internal_get_data() {
        return data;
    }

public:
    foo(int const d = 0) : data(d) {
    }

    // void update(int const d) {
    void increment() {
        std::lock_guard<std::mutex> lock(mx);
        // internal_update(d);
        internal_increment();
    }

    // int update_with_return(int const d) {
    int update_with_return() {
        std::lock_guard<std::mutex> lock(mx);
        auto temp = data;
        // internal_update(d);
        internal_increment();
        return temp;
    }

    int get_data() {
        std::lock_guard<std::mutex> lock(mx);
        return data;
    }
};

void thread_func_recursive_mutex(foo_recursive &fr)
{
    std::this_thread::sleep_for(2s);
    fr.increment();
}

void thread_func_mutex(foo &f)
{
    std::this_thread::sleep_for(2s);
    f.increment();
}

void execute()
{
    // use a recursive mutex
    {
        foo_recursive fr;

        print_time("foo_recursive begin: ");
        std::vector<std::thread> threads;
        for (int i = 0; i < 200; ++i) {
            threads.emplace_back(thread_func_recursive_mutex, std::ref(fr));
        }

        for (auto &t : threads) {
            t.join();
        }
        print_time("foo_recursive end: ");

        std::cout << fr.get_data() << std::endl;
    }

    // using a non-recursive mutex
    {
        foo f;

        print_time("foo begin: ");
        std::vector<std::thread> threads;
        for (int i = 0; i < 200; ++i) {
            threads.emplace_back(thread_func_mutex, std::ref(f));
        }

        for (auto &t : threads) {
            t.join();
        }
        print_time("foo end: ");

        std::cout << f.get_data() << std::endl;
    }
}

}
