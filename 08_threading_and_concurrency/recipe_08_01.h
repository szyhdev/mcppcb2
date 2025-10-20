#pragma once

#include "recipe_08_common.h"

#include <thread>

namespace recipe_08_01
{

using namespace std::chrono;

void func1()
{
    std::cout << "func1: thread func without params" << std::endl;
}

void func2(int const i, double const d, std::string const s)
{
    std::cout << "func2: thread func with params (" <<
            i << ", " << d << ", " << s << ")" << std::endl;
}

void func3()
{
    std::cout << "func3: detached thread (id = 0x" << std::hex <<
            std::this_thread::get_id() << std::dec << ")" << std::endl;
}

void func4(int &i)
{
    i *= 2;
    std::cout << "func4: thread func with params by reference (" <<
            i << ")" << std::endl;
}

void func5()
{
    print_time("func5 begin: ");
    std::this_thread::sleep_for(2s);
    print_time("func5 end: ");
}

void func6()
{
    print_time("func6 begin: ");
    std::this_thread::sleep_until(std::chrono::system_clock::now() + 2s);
    print_time("func6 end: ");
}

void func7(std::chrono::seconds timeout)
{
    print_time("func7 begin: ");
    auto now = std::chrono::system_clock::now();
    auto then = now + timeout;
    do {
        std::this_thread::yield();
    } while (std::chrono::system_clock::now() < then);
    print_time("func7 end: ");
}

void execute()
{
    // create an std::thread object without starting execution
    {
        std::thread t;
    }

    // pass a function to start execution
    {
        std::thread t1(func1);
        t1.join();

        std::thread t2([]() {
            std::cout << "lambda1: thread func without params" << std::endl;
        });
        t2.join();
    }

    // pass a function with arguments to start execution
    {
        std::thread t1(func2, 42, 42.0, "42");
        t1.join();

        std::thread t2([] (int const i, double const d, std::string const s) {
            std::cout << "lambda2: thread func with params (" <<
                    i << ", " << d << ", " << s << ")" << std::endl;
        }, 42, 42.0, "42");
        t2.join();
    }

    // allow a thread to continue its execution independently
    {
        std::thread t(func3);
        t.detach();
    }

    // pass a function with arguments by reference to start execution
    {
        int n = 42;
        std::thread t1(func4, std::ref(n));
        t1.join();

        n = 42;
        std::thread t2([] (int &i) {
            i *= 2;
            std::cout << "lambda3: thread func with params by reference (" <<
                    i << ")" << std::endl;
        }, std::ref(n));
        t2.join();
    }

    // stop execution of a thread for a specified duration
    {
        std::thread t(func5);
        t.join();
    }

    // stop execution of a thread until a specified moment in time
    {
        std::thread t(func6);
        t.join();
    }

    // suspend execution of the current thread
    {
        std::thread t(func7, std::chrono::seconds(2));
        t.join();
    }
}

}
