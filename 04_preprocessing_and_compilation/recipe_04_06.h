#pragma once

#include "recipe_04_common.h"

#define LOAD_AFTER_STORE

namespace recipe_04_06
{

[[nodiscard]] int get_value1()
{
    return 42;
}

[[nodiscard("returned pointer must be managed or freed")]]
void *allocate_memory(size_t size)
{
    return static_cast<void *>(new char [size]);
}

enum class [[nodiscard]] ReturnCodes
{
    OK, NoData, Error
};

ReturnCodes get_value2()
{
    return ReturnCodes::OK;
}

struct [[nodiscard]] Item
{
};

Item get_value3()
{
    return Item {};
}

[[deprecated("use func2 instead")]] void func()
{
}

class [[deprecated]] foo
{
};

double run([[maybe_unused]] int a, double b)
{
    return 2 * b;
}

void option1()
{
}

void option2()
{
}

#ifdef HAS_C20_ATTRIBUTES
void execute_command(char cmd)
{
    switch (cmd) {
    [[likely]] case 'a':    // add
        break;
    [[unlikely]] case 'd':  // delete
        break;
    case 'p':               // print
        break;
    default:
        break;
    }
}
#endif

int shared_data = 0;
std::atomic<int *> data_ptr { nullptr };

void process_without_hint(int *val) {
    if (val) {
        // memory fence instructions are probably inserted here
        std::cout << *val << std::endl;
    }
}

void process_with_hint(int *val [[carries_dependency]])
{
    if (val) {
        std::cout << *val << std::endl;
    }
}

int *get_data [[carries_dependency]]()
{
    int *p = data_ptr.load(std::memory_order_consume);
    return p;
}

void writer_thread()
{
    shared_data = 42;

    data_ptr.store(&shared_data, std::memory_order_release);
#if defined(LOAD_AFTER_STORE)
    data_ptr.notify_one();
#endif
}

void reader_thread()
{
#if defined(LOAD_AFTER_STORE)
    int *local_ptr = nullptr;
    while ((local_ptr = data_ptr.load(std::memory_order_consume)) == nullptr) {
        data_ptr.wait(local_ptr);
    }
#else
    int *local_ptr = data_ptr.load(std::memory_order_consume);
#endif

    if (local_ptr) {
        // case 1: no memory fence instructions for direct pointer dereference
        std::cout << *local_ptr << std::endl;

        // case 2: probably insert memory fence instructions without hint
        process_without_hint(local_ptr);

        // case 3: no memory fence instructions with hint on parameter
        process_with_hint(local_ptr);

        // case 4: no memory fence instructions with hint on return value
        int *ptr_from_func = get_data();
        if (ptr_from_func) {
             std::cout << *ptr_from_func << std::endl;
        }
    }
}

struct default_policy
{
};

template <typename P>
struct container
{
    int size;

    [[no_unique_address]]
    P policy;
};

void execute()
{
    // [[nodiscard]] attribute
    {
        get_value1();
        allocate_memory(1024);
        get_value2();
        get_value3();
    }

    // [[deprecated]] attribute
    {
        func();
        foo f;
    }

    // [[maybe_unused]] attribute
    {
        run(2, 42.0);

        [[maybe_unused]] auto i = get_value1();
    }

    // [[fallthrough]] attribute
    {
        int alternative = get_value1();
        switch (alternative) {
        case 1:
            option1();
            [[fallthrough]];
        case 2:
            option2();
        }
    }

    // [[likely]] and [[unlikely]] attribute
    {
        char cmd;
        switch (cmd) {
        [[likely]] case 'a':    // add
            break;
        [[unlikely]] case 'd':  // delete
            break;
        case 'p':               // print
            break;
        default:
            break;
        }
    }

    // example of [[carries_dependency]]
    {
        std::thread t1(reader_thread);
        std::thread t2(writer_thread);

        t1.join();
        t2.join();
        std::cout << std::endl;
    }

    // example of [[no_unique_address]]
    {
        container<default_policy> c;
        std::cout << sizeof(c) << std::endl;
    }
}

}
