#pragma once

#include "recipe_08_common.h"

#include <thread>

namespace recipe_08_12
{

using namespace std::chrono_literals;

#ifdef HAS_JTHREAD
void thread_func1(int i)
{
    while (i-- > 0) {
        std::this_thread::sleep_for(200ms);
        std::cout << i << ' ';
    }
    std::cout << std::endl;
}

void thread_func2(std::stop_token st, int &i)
{
    while (!st.stop_requested() && (i < 100)) {
        std::this_thread::sleep_for(200ms);
        i++;
    }

    std::cout << "thread_func2: " << i << std::endl;  // prints 5
}
#endif

void execute()
{
#ifdef HAS_JTHREAD
    // automatically join a thread when it goes out of scope
    {
        {
            std::jthread t(thread_func1, 10);
        }
        std::cout << std::endl;
    }

    // cancel execution of a thread
    {
        int a = 0;
        std::jthread t(thread_func2, std::ref(a));

        std::this_thread::sleep_for(1s);
        t.request_stop();

        std::cout << "thread_main:  " << a << std::endl;  // prints 4
        t.join();
        std::cout << std::endl;
    }

    // cancel work of multiple threads
    {
        int a = 0;
        int b = 10;

        std::stop_source ss;
        std::jthread t1(thread_func2, ss.get_token(), std::ref(a));
        std::jthread t2(thread_func2, ss.get_token(), std::ref(b));

        std::this_thread::sleep_for(1s);
        ss.request_stop();

        std::cout << "thread_main:  " << a << ", " << b << std::endl;  // prints 4 and 14
        t1.join();
        t2.join();
        std::cout << std::endl;
    }

    // execute a piece of code when cancelling thread
    {
        int a = 0;

        std::stop_source src;
        std::stop_token token = src.get_token();
        std::stop_callback cb(token, [] {
            std::cout << "callback: done" << std::endl;
        });
        std::jthread t(thread_func2, token, std::ref(a));

        std::this_thread::sleep_for(1s);
        src.request_stop();

        std::cout << "thread_main:  " << a << std::endl;  // prints 4
    }
#endif
}

}
