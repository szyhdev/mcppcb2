#pragma once

#include "recipe_08_common.h"

#include <atomic>
#include <future>
#include <random>

namespace recipe_08_08
{

using namespace std::chrono_literals;

#ifdef HAS_ATOMIC_REF
void do_count(int &c)
{
    std::atomic_ref<int> counter { c };

    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&counter] () {
            for (int i = 0; i < 10; ++i) {
                ++counter;
            }
        });
    }

    for (auto &t : threads) {
        t.join();
    }
}
#endif

std::atomic<int> int_value { 0 };
std::mutex io_mutex;

void set_int_value(int x)
{
    int_value.store(x, std::memory_order_relaxed);      // set value atomically

    {
        std::lock_guard<std::mutex> lock(io_mutex);
        std::cout << "int value stored: " << x << std::endl;
    }
}

void get_int_value()
{
    int x;
    do {
        x = int_value.load(std::memory_order_relaxed);  // get value atomically
    } while (x == 0);

    {
        std::lock_guard<std::mutex> lock(io_mutex);
        std::cout << "int value loaded: " << x << std::endl;
    }
}

std::atomic<bool> ready(false);
std::atomic<bool> winner(false);

void summation(int id)
{
    while (!ready.load()) {
    }

    long total = 0;
    for (int i = 0; i < 100000000; ++i) {
        total += i;
    }

    if (!winner.exchange(true)) {
        std::cout << "thread " << id << " won: " << total << std::endl;
    }
}

std::vector<int> generate_random()
{
    std::random_device rd;
    auto generator = std::mt19937 { rd() };
    auto dist = std::uniform_int_distribution<> { 1, 99 };
    std::vector<int> numbers(10000000, 0);
    std::generate(std::begin(numbers), std::end(numbers),
            [&dist, &generator] {
                return dist(generator);
            });

    return numbers;
}

template <typename T,
        typename I = typename std::enable_if_t<std::is_integral_v<T>>>
class atomic_counter
{
private:
    std::atomic<T> counter { 0 };

public:
    T increment() {
        return counter.fetch_add(1);
    }

    T decrement() {
        return counter.fetch_sub(1);
    }

    T get() {
        return counter.load();
    }
};

void execute()
{
    // use std::atomic to create atomic objects
    {
        std::atomic<int> counter { 0 };

        std::vector<std::thread> threads;
        for (int i = 0; i < 10; ++i) {
            threads.emplace_back([&counter] () {
                for (int i = 0; i < 10; ++i) {
                    ++counter;
                }
            });
        }

        for (auto &t : threads) {
            t.join();
        }

        std::cout << counter << std::endl;
    }

#ifdef HAS_ATOMIC_REF
    // use std::atomic_ref to apply atomic operations to a referenced object
    {
        int counter = 0;
        do_count(counter);
        std::cout << counter << std::endl;
    }
#endif

    // use std::atomic_flag for an atomic Boolean type
    {
        std::atomic_flag lock = ATOMIC_FLAG_INIT;  // this macro has been deprecated in C++20
        int counter = 0;

        std::vector<std::thread> threads;
        for (int i = 0; i < 10; ++i) {
            threads.emplace_back([&] () {
                for (int i = 0; i < 10; ++i) {
                    while (lock.test_and_set(std::memory_order_acquire));
                    ++counter;
                    lock.clear(std::memory_order_release);
                }
            });
        }

        for (auto &t : threads) {
            t.join();
        }

        std::cout << counter << std::endl;
        std::cout << std::endl;
    }

    // use atomic type's member functions: load, store and exchange
    {
        // load and store
        std::thread t1(get_int_value);
        std::thread t2(set_int_value, 123);
        t1.join();
        t2.join();

        // exchange
        std::vector<std::thread> threads;
        for (int i = 0; i < 10; ++i) {
            threads.push_back(std::thread(summation, i));
        }
        ready = true;
        for (auto& t : threads) {
            t.join();
        }
        std::cout << std::endl;
    }

    // use atomic type's member functions: fetch_add and fetch_sub
    {
        std::atomic<int> sum { 0 };
        std::vector<int> numbers = generate_random();

        // auto start_expected = std::chrono::high_resolution_clock::now();
        // auto sum_expected = std::accumulate(std::begin(numbers), std::end(numbers), 0);
        // auto end_expected = std::chrono::high_resolution_clock::now();

        // auto start = std::chrono::high_resolution_clock::now();
        size_t size = numbers.size();
        std::vector<std::thread> threads;
        for (int i = 0; i < 10; ++i) {
            threads.emplace_back([&sum, &numbers] (size_t const start, size_t const end) {
                for (size_t i = start; i < end; ++i) {
                    std::atomic_fetch_add_explicit(&sum, numbers[i],
                            std::memory_order_acquire);
                }
            }, i * (size / 10), (i + 1) * (size / 10));
        }

        for (auto &t : threads) {
            t.join();
        }
        // auto end = std::chrono::high_resolution_clock::now();

        // std::cout << sum_expected << ": " << (end_expected - start_expected).count() <<
        //         "ns" << std::endl;
        // std::cout << sum << ": " << (end - start).count() <<
        //         "ns" << std::endl;
        std::cout << "sum = " << sum << std::endl;
        std::cout << std::endl;
    }

    // implement a class to represent an atomic counter
    {
        atomic_counter<int> counter;

        std::vector<std::thread> threads;
        for (int i = 0; i < 10; ++i) {
            threads.emplace_back([&counter] () {
                for (int i = 0; i < 10; ++i) {
                    counter.increment();
                }
            });
        }

        for (auto &t : threads) {
            t.join();
        }

        std::cout << counter.get() << std::endl;
        std::cout << std::endl;
    }

    // an efficient thread-synchronization mechanism provided in C++20
    {
        std::atomic<bool> all_done { false };
        std::atomic<unsigned> completed {};
        std::atomic<unsigned> uncompleted { 16 };
        std::future<void> task_futures[16];

        for (std::future<void> &task_future : task_futures) {
            task_future = std::async([&] () {
                std::this_thread::sleep_for(50ms);
                ++completed;
                --uncompleted;
                if (uncompleted.load() == 0) {
                    all_done = true;
                    all_done.notify_one();
                }
            });
        }

        all_done.wait(false);
        std::cout << "all " << completed.load() << " tasks done" << std::endl;
    }
}

}
