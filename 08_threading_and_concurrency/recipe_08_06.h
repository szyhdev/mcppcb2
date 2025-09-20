#pragma once

#include <iostream>
#include <future>

namespace recipe_08_06
{

using namespace std::chrono_literals;

std::mutex g_mutex;

void produce_value(std::promise<int> &p)
{
    std::cout << "produce_value: entering" << std::endl;

    // simulate long running operation
    {
        std::this_thread::sleep_for(2s);
    }

    p.set_value(42);

    std::cout << "produce_value: value set" << std::endl;
}

void consume_value(std::future<int> &f)
{
    std::cout << "consume_value: entering" << std::endl;

    auto value = f.get();

    std::cout << "consume_value: value got" << std::endl;

    std::lock_guard<std::mutex> lock(g_mutex);
    std::cout << value << std::endl;
}

void produce_exception(std::promise<int> &p)
{
    std::cout << "produce_exception: entering" << std::endl;

    // simulate long running operation
    {
        std::this_thread::sleep_for(2s);
    }

    try {
       std::cout << "produce_exception: exception occurred" << std::endl;
        throw std::runtime_error("an error has occurred!");
    }
    catch (...) {
        p.set_exception(std::current_exception());
    }
}

void consume_exception(std::future<int> &f)
{
    std::cout << "consume_exception: entering" << std::endl;

    std::lock_guard<std::mutex> lock(g_mutex);
    try {
        std::cout << f.get() << std::endl;
    }
    catch (std::exception const &e) {
        std::cout << "consume_exception: exception got" << std::endl;
        std::cout << "exception: " << e.what() << std::endl;
    }
}

void execute()
{
    // communicate a value from one thread to another
    {
        std::promise<int> p;
        std::thread t1(produce_value, std::ref(p));

        std::future<int> f = p.get_future();
        std::thread t2(consume_value, std::ref(f));

        t1.join();
        t2.join();
    }

    // communicate an exception from one thread to another
    {
        std::promise<int> p;
        std::thread t1(produce_exception, std::ref(p));

        std::future<int> f = p.get_future();
        std::thread t2(consume_exception, std::ref(f));

        t1.join();
        t2.join();
    }
}

}
