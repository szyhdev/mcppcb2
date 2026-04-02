#pragma once

#include "recipe_08_common.h"

namespace recipe_08_13
{

using namespace std::chrono_literals;

#ifdef HAS_LATCHES_BARRIERS
void process(std::vector<int> const &data)
{
    for (auto const e : data) {
        std::cout << e << " ";
    }
    std::cout << std::endl;
}

int create(int const i, int const factor)
{
    return i * factor;
}
#endif

void execute()
{
#ifdef HAS_LATCHES_BARRIERS
    // use std::latch to make threads wait until a counter,
    // decreased by other threads, reaches zero
    {
        int const jobs = 4;
        std::vector<int> data(jobs);
        std::latch work_done(jobs);

        std::vector<std::jthread> threads;
        for (int i = 0; i < jobs; ++i) {
            threads.push_back(std::jthread([&data, i, &work_done] {
                std::this_thread::sleep_for(1s);
                data[i] = create(i, 2);
                work_done.count_down();
            }));
        }

        work_done.wait();
        process(data);
        std::cout << std::endl;
    }

    // use std::barrier to perform loop synchronization between parallel tasks
    {
        int const jobs = 4;
        std::vector<int> data(jobs);

        int cycle = 0;
        std::stop_source ss;
        std::barrier<std::function<void()>> work_done(
                jobs,
                [&data, &cycle, &ss] () {  // completion function
                    process(data);
                    cycle++;
                    if (cycle == 10) {
                        ss.request_stop();
                    }
                });

        std::vector<std::jthread> threads;
        for (int i = 0; i < jobs; ++i) {
            threads.push_back(std::jthread(
                    [&data, &cycle, &work_done] (std::stop_token st, int const i) {
                        while (!st.stop_requested()) {
                            std::this_thread::sleep_for(200ms);
                            data[i] = create(i, cycle);
                            work_done.arrive_and_wait();
                        }
                    }, ss.get_token(), i));
        }

        for (auto &t : threads) {
            t.join();
        }
        std::cout << std::endl;
    }

    // use std::counting_semaphore<N> or std::binary_semaphore to restrict
    // a number of threads (a single thread, in the case of binary_semaphore)
    // to access a shared resource, or pass notifications between different threads
    {
        int const jobs = 4;
        std::vector<int> data;
        std::binary_semaphore bs(1);

        std::vector<std::jthread> threads;
        for (int i = 1; i <= jobs; ++i) {
            threads.push_back(std::jthread([&data, i, &bs] () {
                for (int k = 1; k < 5; ++k) {
                    std::this_thread::sleep_for(200ms);
                    int value = create(i, k);
                    bs.acquire();
                    data.push_back(value + i * 100);
                    bs.release();
                }
            }));
        }

        for (auto &t : threads) {
            t.join();
        }

        process(data);
    }

#endif
}

}
