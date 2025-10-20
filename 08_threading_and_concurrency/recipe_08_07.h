#pragma once

#include <iostream>
#include <future>

namespace recipe_08_07
{

using namespace std::chrono_literals;

std::mutex g_mutex;

void do_something()
{
    // simulate long running operation
    {
        std::this_thread::sleep_for(2s);
    }

    std::lock_guard<std::mutex> lock(g_mutex);
    std::cout << "operation 1 done" << std::endl;
}

void do_something_else()
{
    // simulate long running operation
    {
        std::this_thread::sleep_for(1s);
    }

    std::lock_guard<std::mutex> lock(g_mutex);
    std::cout << "operation 2 done" << std::endl;
}

int compute_something()
{
    // simulate long running operation
    {
        std::this_thread::sleep_for(2s);
    }

    std::cout << "computation 1 done" << std::endl;
    return 42;
}

int compute_something_else()
{
    // simulate long running operation
    {
        std::this_thread::sleep_for(1s);
    }

    std::cout << "computation 2 done" << std::endl;
    return 24;
}

void execute()
{
    // execute a function asynchronously on another thread when current thread
    // continues its execution without expecting a result
    {
        auto f = std::async(std::launch::async, do_something);
        do_something_else();
        f.wait();

        std::cout << "both operations done" << std::endl;
        std::cout << std::endl;
    }

    // execute a function asynchronously on another thread when current thread
    // continues its execution, until the result is needed
    {
        auto f = std::async(std::launch::async, compute_something);
        auto value = compute_something_else();
        value += f.get();

        std::cout << "both computations done: " << value << std::endl;
        std::cout << std::endl;
    }

    // specify a duration after which the call ends and returns even if
    // the shared state is not yet available through the future
    {
        auto f = std::async(std::launch::async, do_something);

        while (true) {
            auto status = f.wait_for(500ms);
            if (status == std::future_status::ready) {
                break;
            }

            std::cout << "waiting..." << std::endl;
        }
    }
}

}
