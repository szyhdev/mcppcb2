#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <queue>
#include <random>
#include <array>
#include <vector>
#include <algorithm>

namespace recipe_08_05
{

using namespace std::chrono_literals;

std::mutex print_mutes;
std::mutex queue_mutex;
std::condition_variable buffer_cv;
std::queue<int> buffer;
bool done;

void producer(int const id, std::mt19937 &generator,
        std::uniform_int_distribution<int> &sleep_dist,
        std::uniform_int_distribution<int> &data_dist)
{
    for (int i = 0; i < 5; ++i) {
        // simulate work
        std::this_thread::sleep_for(std::chrono::seconds(sleep_dist(generator)));

        // generate data
        int value = id * 100 + data_dist(generator);

        {
            std::unique_lock<std::mutex> locker(print_mutes);
            std::cout << "[produced]: " << value << std::endl;
        }

        // notify consumers
        {
            std::unique_lock<std::mutex> locker(queue_mutex);
            buffer.push(value);
            buffer_cv.notify_one();
        }
    }
}

void consumer()
{
    // loop until end is signaled
    while (!done) {
        std::unique_lock<std::mutex> locker(queue_mutex);

        buffer_cv.wait_for(locker, std::chrono::seconds(1),
                [&] () {
                    return !buffer.empty();
                });

        // if there are values in the queue process them
        while (!done && !buffer.empty()) {
            std::unique_lock<std::mutex> locker(print_mutes);
            std::cout << "[consumed]: " << buffer.front() << std::endl;
            buffer.pop();
        }
    }
}

std::mutex mutex_any;
std::condition_variable_any cv_any;
int cargo = 0;

bool shipment_available()
{
    return cargo != 0;
}

void cargo_producer(int n)
{
    for (int i = 0; i < n; ++i) {
        while (shipment_available()) {
            std::this_thread::yield();
        }

        mutex_any.lock();
        cargo = i + 1;
        cv_any.notify_one();
        mutex_any.unlock();
    }
}

void cargo_consumer(int n)
{
    for (int i = 0; i < n; ++i) {
        mutex_any.lock();
        cv_any.wait(mutex_any, shipment_available);

        std::cout << cargo << std::endl;
        cargo = 0;
        mutex_any.unlock();
    }
}

void execute()
{
    // synchronize threads with notifications on condition variables
    {
        std::condition_variable cv;

        std::mutex cv_mutex;  // data mutex
        std::mutex io_mutex;  // I/O mutex

        int data = 0;

        std::thread p([&] () {  // producer
            // simulate long running operation
            {
                std::this_thread::sleep_for(2s);
            }

            // produce
            {
                std::unique_lock lock(cv_mutex);
                data = 42;
            }

            // print message
            {
                std::lock_guard l(io_mutex);
                std::cout << "produced " << data << std::endl;
            }

            cv.notify_one();
        });

        std::thread c([&] () {  // consumer
            // wait for notification
            {
                std::unique_lock lock(cv_mutex);
                cv.wait(lock);
            }

            // print message
            {
                std::lock_guard lock(io_mutex);
                std::cout << "consumed " << data << std::endl;
            }
        });

        p.join();
        c.join();

        std::cout << std::endl;
    }

    // a similar but more complex example than the above one
    {
        auto seed_data = std::array<int, std::mt19937::state_size> {};
        std::random_device rd {};

        std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
        std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
        auto generator = std::mt19937 { seq };
        auto sleep_dist = std::uniform_int_distribution<> { 1, 5 };
        auto data_dist = std::uniform_int_distribution<> { 1, 99 };

        std::thread consumer_thread(consumer);

        std::vector<std::thread> producer_threads;
        for (int i = 0; i < 5; ++i) {
            producer_threads.emplace_back(producer, i + 1, std::ref(generator),
                    std::ref(sleep_dist), std::ref(data_dist));
        }

        // wait for the worker threads to finish
        for (auto &t : producer_threads) {
            t.join();
        }

        // notify the logger to finish and wait for it
        done = true;
        consumer_thread.join();
        std::cout << std::endl;
    }

    // use condition_variable_any::wait with predicate
    {
        std::thread producer_thread (cargo_producer, 10);
        std::thread consumer_thread (cargo_consumer, 10);

        producer_thread.join();
        consumer_thread.join();
    }
}

}
