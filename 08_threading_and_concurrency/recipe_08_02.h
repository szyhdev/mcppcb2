
#pragma once

#include "recipe_08_common.h"

#include <mutex>
#include <thread>
#include <vector>

namespace recipe_08_02
{

using namespace std::chrono_literals;

std::mutex g_mutex;

void thread_func_1()
{
    std::lock_guard<std::mutex> lock(g_mutex);
    std::cout << "running thread " << std::this_thread::get_id() << std::endl;
    std::this_thread::yield();
    std::this_thread::sleep_for(1s);
    std::cout << "done in thread " << std::this_thread::get_id() << std::endl;
}

void thread_func_2()
{
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        std::cout << "running thread " << std::this_thread::get_id() << std::endl;
    }

    std::this_thread::yield();
    std::this_thread::sleep_for(1s);

    {
        std::lock_guard<std::mutex> lock(g_mutex);
        std::cout << "done in thread " << std::this_thread::get_id() << std::endl;
    }
}

void thread_func_3()
{
    g_mutex.lock();
    std::cout << "running thread " << std::this_thread::get_id() << std::endl;
    std::this_thread::yield();
    std::this_thread::sleep_for(1s);
    std::cout << "done in thread " << std::this_thread::get_id() << std::endl;
    g_mutex.unlock();
}

template <class M>
class lock_guard
{
public:
    typedef M mutex_type;

    explicit lock_guard(M &Mtx) : mtx(Mtx) {
        mtx.lock();
    }

    lock_guard(M &Mtx, std::adopt_lock_t) : mtx(Mtx) {
    }

    ~lock_guard() noexcept {
        mtx.unlock();
    }

    lock_guard(const lock_guard &) = delete;
    lock_guard &operator =(const lock_guard &) = delete;

private:
    M &mtx;
};

template <typename T>
struct container
{
    std::mutex mutex;
    std::vector<T> data;
};

template <typename T>
void move_between_deadlock(container<T> &c1, container<T> &c2, T const value)
{
    std::lock_guard<std::mutex> l1(c1.mutex);
    std::lock_guard<std::mutex> l2(c2.mutex);

    c1.data.erase(std::remove(c1.data.begin(), c1.data.end(), value),
            c1.data.end());
    c2.data.push_back(value);
}

template <typename T>
void move_between(container<T> &c1, container<T> &c2, T const value)
{
    // 1. lock_guard in C++11
    // std::lock(c1.mutex, c2.mutex);
    // std::lock_guard<std::mutex> l1(c1.mutex, std::adopt_lock);
    // std::lock_guard<std::mutex> l2(c2.mutex, std::adopt_lock);

    // 2. scope_lock in C++17
    std::scoped_lock lock(c1.mutex, c2.mutex);

    c1.data.erase(std::remove(c1.data.begin(), c1.data.end(), value),
            c1.data.end());
    c2.data.push_back(value);
}

void execute()
{
    // acquire a lock on mutex with std::lock_guard when entering the thread
    {
        std::vector<std::thread> threads;
        for (int i = 0; i < 3; ++i) {
            threads.emplace_back(thread_func_1);
        }

        for (auto &t : threads) {
            t.join();
        }

        std::cout << std::endl;
    }

    // acquire a lock on mutex with std::lock_guard when accessing shared resource
    {
        std::vector<std::thread> threads;
        for (int i = 0; i < 3; ++i) {
            threads.emplace_back(thread_func_2);
        }

        for (auto &t : threads) {
            t.join();
        }

        std::cout << std::endl;
    }

    // acquire a lock on mutex with lock()
    {
        std::vector<std::thread> threads;
        for (int i = 0; i < 3; ++i) {
            threads.emplace_back(thread_func_3);
        }

        for (auto &t : threads) {
            t.join();
        }

        std::cout << std::endl;
    }

    // synchronize access to multiple shared resources to avoid deadlocks
    {
        container<int> c1;
        c1.data.push_back(1);
        c1.data.push_back(2);
        c1.data.push_back(3);
        recipe_common::print_collection(c1.data, "c1.data: ");

        container<int> c2;
        c2.data.push_back(4);
        c2.data.push_back(5);
        c2.data.push_back(6);
        recipe_common::print_collection(c2.data, "c2.data: ");

        // 1. deadlock manner
        // std::thread t1(move_between_deadlock<int>, std::ref(c1), std::ref(c2), 3);
        // std::thread t2(move_between_deadlock<int>, std::ref(c2), std::ref(c1), 6);

        // 2. deadlock avoidance manner
        std::thread t1(move_between<int>, std::ref(c1), std::ref(c2), 3);
        std::thread t2(move_between<int>, std::ref(c2), std::ref(c1), 6);

        t1.join();
        t2.join();

        recipe_common::print_collection(c1.data, "c1.data: ");
        recipe_common::print_collection(c2.data, "c2.data: ");
    }
}

}
