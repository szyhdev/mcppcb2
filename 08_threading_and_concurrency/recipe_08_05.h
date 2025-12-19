#pragma once

#include "recipe_08_common.h"

#include <condition_variable>
#include <random>

namespace recipe_08_05
{

using namespace std::chrono_literals;

std::condition_variable buffer_cv;
std::mutex buffer_mutex;
std::queue<int> buffer;

std::mutex print_mutex;
bool all_done;

void producer(int const id, std::mt19937 &generator,
        std::uniform_int_distribution<int> &sleep_dist,
        std::uniform_int_distribution<int> &data_dist)
{
    for (int i = 0; i < 5; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(sleep_dist(generator)));

        int value = id * 100 + data_dist(generator);

        {
            std::unique_lock<std::mutex> buffer_lock(buffer_mutex);
            buffer.push(value);
        }

        {
            std::unique_lock<std::mutex> print_lock(print_mutex);
            std::cout << "produced: " << value << std::endl;
        }

        buffer_cv.notify_one();
    }
}

void consumer()
{
    while (!all_done) {
        std::unique_lock<std::mutex> buffer_lock(buffer_mutex);

        buffer_cv.wait_for(buffer_lock, std::chrono::seconds(1),
                [&] () {
                    return !buffer.empty();
                });

        while (!all_done && !buffer.empty()) {
            std::unique_lock<std::mutex> print_lock(print_mutex);
            std::cout << "consumed: " << buffer.front() << std::endl;
            buffer.pop();
        }
    }
}

void execute()
{
    // synchronize threads with notifications on condition variables
    {
        std::condition_variable data_cv;

        std::mutex data_mutex;
        std::mutex io_mutex;

        int data = 0;

        std::thread producer([&] () {
            std::this_thread::sleep_for(1s);

            {
                std::unique_lock lock(data_mutex);
                data = 42;
            }

            {
                std::lock_guard l(io_mutex);
                std::cout << "produced: " << data << std::endl;
            }

            data_cv.notify_one();
        });

        std::thread consumer([&] () {
            {
                std::unique_lock lock(data_mutex);
                data_cv.wait(lock);
            }

            {
                std::lock_guard lock(io_mutex);
                std::cout << "consumed: " << data << std::endl;
            }
        });

        producer.join();
        consumer.join();

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
        all_done = true;
        consumer_thread.join();
    }
}

}
