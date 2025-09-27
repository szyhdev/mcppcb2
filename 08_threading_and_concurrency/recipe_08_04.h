#pragma once

#include <iostream>
#include <exception>
#include <thread>
#include <vector>

namespace recipe_08_04
{

std::mutex except_mutex;
std::vector<std::exception_ptr> except_vec;

void func1()
{
    throw std::runtime_error("exception 1");
}

void func2()
{
    throw std::runtime_error("exception 2");
}

void thread_func1()
{
    try {
        func1();
    }
    catch (...) {
        std::lock_guard<std::mutex> lock(except_mutex);
        except_vec.push_back(std::current_exception());
    }
}

void thread_func2()
{
    try {
        func2();
    }
    catch (...) {
        std::lock_guard<std::mutex> lock(except_mutex);
        except_vec.push_back(std::current_exception());
    }
}

void execute()
{
    // handle exceptions thrown in a worker thread from main thread
    {
        except_vec.clear();

        std::thread t1(thread_func1);
        std::thread t2(thread_func2);
        t1.join();
        t2.join();

        for (auto const &ex : except_vec) {
            try {
                if (ex != nullptr) {
                    std::rethrow_exception(ex);
                }
            }
            catch (std::exception const &ex) {
                std::cout << ex.what() << std::endl;
            }
        }
    }
}

}
