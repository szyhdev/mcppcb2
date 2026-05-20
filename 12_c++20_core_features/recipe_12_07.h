#pragma once

#include "recipe_12_common.h"

namespace recipe_12_07
{

struct promise_base
{
    auto initial_suspend() noexcept {
        recipe_12_common::print_debug_info("promise_base::initial_suspend");
        return std::suspend_always {};
    }

    auto final_suspend() noexcept {
        recipe_12_common::print_debug_info("promise_base::final_suspend");
        return std::suspend_always {};
    }

    void unhandled_exception() {
        recipe_12_common::print_debug_info("promise_base::unhandled_exception");
        std::terminate();
    }
};

template <typename T>
struct promise final : public promise_base
{
    auto get_return_object() {
        auto handle = std::coroutine_handle<promise<T>>::from_promise(*this);
        recipe_12_common::print_debug_info("promise<T>::get_return_object: ", &handle.promise());
        return handle;
    }

    template <typename V, typename = std::enable_if_t<std::is_convertible_v<V &&, T>>>
    void return_value(V &&value) noexcept(std::is_nothrow_constructible_v<T, V &&>) {
        recipe_12_common::print_debug_info("promise<T>::return_value");
        value_ = value;
    }

    T get_value() const noexcept {
        recipe_12_common::print_debug_info("promise<T>::get_value");
        return value_;
    }

private:
    T value_;
};

template <>
struct promise<void> final : public promise_base
{
    auto get_return_object() {
        auto handle = std::coroutine_handle<promise<void>>::from_promise(*this);
        recipe_12_common::print_debug_info("promise<void>::get_return_object: ", &handle.promise());
        return handle;
    }

    void return_void() noexcept {
        recipe_12_common::print_debug_info("promise<void>::return_void");
    }
};

template <typename T>
struct promise<T &> final : public promise_base
{
    auto get_return_object() {
        auto handle = std::coroutine_handle<promise<T &>>::from_promise(*this);
        recipe_12_common::print_debug_info("promise<T &>::get_return_object: ", &handle.promise());
        return handle;
    }

    void return_value(T &value) noexcept {
        recipe_12_common::print_debug_info("promise<T &>::return_value");
        value_ = std::addressof(value);
    }

    T &get_value() const noexcept {
        recipe_12_common::print_debug_info("promise<T &>::get_value");
        return *value_;
    }

private:
    T *value_ = nullptr;
};

template <typename T>
struct task_awaiter
{
    using promise_type = promise<T>;

    task_awaiter(std::coroutine_handle<promise_type> handle) noexcept
            : handle_(handle) {
        recipe_12_common::print_debug_info("task_awaiter::task_awaiter");
    }

    bool await_ready() const noexcept {
        recipe_12_common::print_debug_info("task_awaiter::await_ready");
        return !handle_ || handle_.done();
    }

    void await_suspend([[maybe_unused]] std::coroutine_handle<> continuation) noexcept {
        recipe_12_common::print_debug_info("task_awaiter::await_suspend");
        handle_.resume();
    }

    decltype(auto) await_resume() {
        recipe_12_common::print_debug_info("task_awaiter::await_resume");
        if (!handle_) {
            throw std::runtime_error { "broken promise" };
        }
        return handle_.promise().get_value();
    }

private:
    std::coroutine_handle<promise_type> handle_;
};

template <typename T>
struct task
{
    using promise_type = promise<T>;

    task(std::coroutine_handle<promise_type> handle) : handle_(handle) {
        recipe_12_common::print_debug_info("task::task: ", &handle_.promise());
    }

    ~task() {
        recipe_12_common::print_debug_info("task::~task: ", &handle_.promise());
        if (handle_) {
            handle_.destroy();
        }
    }

    task(task &&t) noexcept : handle_(t.handle_) {
        recipe_12_common::print_debug_info("task::task(task &&)");
        t.handle_ = nullptr;
    }

    task &operator =(task &&other) noexcept {
        recipe_12_common::print_debug_info("task::operator =");
        if (std::addressof(other) != this) {
            if (handle_) {
                handle_.destroy();
            }
            handle_ = other.handle_;
            other.handle_ = nullptr;
        }
        return *this;
    }

    task(task const &) = delete;
    task &operator =(task const &) = delete;

    bool is_done() noexcept {
        recipe_12_common::print_debug_info("task::is_done: ", &handle_.promise());
        return !handle_ || handle_.done();
    }

    void resume() noexcept {
        recipe_12_common::print_debug_info("task::resume: ", &handle_.promise());
        if (handle_) {
            handle_.resume();
        }
    }

    T value() const noexcept {
        recipe_12_common::print_debug_info("task::value");
        return handle_.promise().get_value();
    }

    auto operator co_await() const & noexcept {
        recipe_12_common::print_debug_info("task::operator co_await");
        return task_awaiter<T> { handle_ };
    }

private:
    std::coroutine_handle<promise_type> handle_ = nullptr;
};

task<void> say_hello()
{
    std::cout << "hello, world!" << std::endl;
    co_return;
}

task<int> get_answer()
{
    co_return 42;
}

task<int &> get_answer(int &a)
{
    a++;
    co_return a;
}

task<void> print_answer()
{
    auto value = co_await get_answer();
    std::cout << value << std::endl;
}

task<int> get_url(int i)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    co_return i * 10;
}

task<void> get_all_urls(int count)
{
    for (int i = 0; i < count; ++i) {
        auto value = co_await get_url(i);
        std::cout << value << " " << std::flush;
    }
    std::cout << std::endl;
}

void execute()
{
    // coroutine without return value
    {
        recipe_12_common::execute(say_hello());
        std::cout << std::endl;
    }

    // coroutine with return value
    {
        auto t = get_answer();
        recipe_12_common::execute(t);
        std::cout << t.value() << std::endl;
    }
    std::cout << std::endl;

    // coroutine with reference parameter and return value
    {
        int a = 42;
        auto t = get_answer(a);
        recipe_12_common::execute(t);
        int &b = t.value();
        assert(&a == &b);
        std::cout << a << std::endl;
        std::cout << b << std::endl;
    }
    std::cout << std::endl;

    // coroutine that calls another coroutine
    {
        recipe_12_common::execute(print_answer());
        std::cout << std::endl;
    }

    // coroutine that simulates concurrent url requests
    {
        recipe_12_common::execute(get_all_urls(10));
    }
}

}
