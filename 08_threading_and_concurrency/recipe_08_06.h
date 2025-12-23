#pragma once

#include "recipe_08_common.h"

#include <future>

namespace recipe_08_06
{

using namespace std::chrono_literals;

void produce_value(std::promise<int> &p)
{
    std::this_thread::sleep_for(2s);
    p.set_value(42);
    std::cout << "produce_value: value set" << std::endl;
}

void consume_value(std::future<int> &f)
{
    auto value = f.get();
    std::cout << "consume_value: value got" << std::endl;
    std::cout << value << std::endl;
}

void produce_exception(std::promise<int> &p)
{
    std::this_thread::sleep_for(2s);
    try {
        std::cout << "produce_exception: exception occurred" << std::endl;
        throw std::runtime_error("error");
    }
    catch (...) {
        p.set_exception(std::current_exception());
    }
}

void consume_exception(std::future<int> &f)
{
    try {
        std::cout << f.get() << std::endl;
    }
    catch (std::exception const &e) {
        std::cout << "consume_exception: exception got" << std::endl;
        std::cout << e.what() << std::endl;
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
        std::cout << std::endl;
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
