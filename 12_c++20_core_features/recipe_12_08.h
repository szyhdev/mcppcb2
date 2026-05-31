#pragma once

#include "recipe_12_common.h"

namespace recipe_12_08
{

template <typename T>
struct generator
{
    struct promise_type
    {
        T const *value_;
        std::exception_ptr exptr_;

        auto get_return_object() {
            auto handle = std::coroutine_handle<promise_type>::from_promise(*this);
            recipe_12_common::print_debug_info("promise_type>::get_return_object: ", &handle.promise());
            return handle;
        }

        auto initial_suspend() noexcept {
            recipe_12_common::print_debug_info("promise_type::initial_suspend");
            return std::suspend_always {};
        }

        auto final_suspend() noexcept {
            recipe_12_common::print_debug_info("promise_type::final_suspend");
            return std::suspend_always {};
        }

        void unhandled_exception() noexcept {
            recipe_12_common::print_debug_info("promise_type::unhandled_exception");
            exptr_ = std::current_exception();
        }

        void rethrow_if_exception() {
            recipe_12_common::print_debug_info("promise_type::rethrow_if_exception");
            if (exptr_) {
                std::rethrow_exception(exptr_);
            }
        }

        auto yield_value(T const &v) {
            recipe_12_common::print_debug_info("promise_type::yield_value");
            value_ = std::addressof(v);
            return std::suspend_always {};
        }

        void return_void() {
            recipe_12_common::print_debug_info("promise_type::return_void");
        }

        template <typename U>
        U &&await_transform(U &&v) {
            recipe_12_common::print_debug_info("promise_type::await_transform");
            return std::forward<U>(v);
        }
    };

    struct iterator
    {
        using iterator_category = std::input_iterator_tag;
        using difference_type = ptrdiff_t;
        using value_type = T;
        using reference = T const &;
        using pointer = T const *;

        std::coroutine_handle<promise_type> handle_ = nullptr;

        iterator() = default;

        iterator(std::nullptr_t) : handle_(nullptr) {
            recipe_12_common::print_debug_info("iterator::iterator(std::nullptr_t): ", this);
        }

        iterator(std::coroutine_handle<promise_type> arg) : handle_(arg) {
            recipe_12_common::print_debug_info("iterator::iterator(std::coroutine_handle<promise_type>): ", this);
        }

        iterator &operator ++() {
            recipe_12_common::print_debug_info("iterator::operator ++: ", this);
            handle_.resume();
            if (handle_.done()) {
                std::exchange(handle_, {}).promise().rethrow_if_exception();
            }
            return *this;
        }

        void operator ++(int) {
            recipe_12_common::print_debug_info("iterator::operator ++(int): ", this);
            ++*this;
        }

        bool operator ==(iterator const &right) const {
            recipe_12_common::print_debug_info("iterator::operator ==: ", this, ", ", &right);
            return handle_ == right.handle_;
        }

        bool operator !=(iterator const &right) const {
            recipe_12_common::print_debug_info("iterator::operator !=: ", this, ", ", &right);
            return !(*this == right);
        }

        reference operator *() const {
            recipe_12_common::print_debug_info("iterator::operator *: ", this);
            return *handle_.promise().value_;
        }

        pointer operator ->() const {
            recipe_12_common::print_debug_info("iterator::operator ->: ", this);
            return handle_.promise().value_;
        }
    };

    generator() = default;

    generator(std::coroutine_handle<promise_type> handle) : handle_(handle) {
        recipe_12_common::print_debug_info("generator::generator(promise_type &)");
    }

    ~generator() {
        recipe_12_common::print_debug_info("generator::~generator");
        if (handle_) {
            handle_.destroy();
        }
    }

    generator(generator &&other) : handle_(other.handle_) {
        recipe_12_common::print_debug_info("generator::generator(generator &&)");
        other.handle_ = nullptr;
    }

    generator &operator =(generator &&other) {
        recipe_12_common::print_debug_info("generator::operator =(generator &&)");
        if (this != std::addressof(other)) {
            handle_ = other.handle_;
            other.handle_ = nullptr;
        }
        return *this;
    }

    generator(generator const &) = delete;
    generator &operator =(generator const &) = delete;

    iterator begin() {
        recipe_12_common::print_debug_info("generator::begin");
        if (handle_) {
            handle_.resume();
            if (handle_.done()) {
                handle_.promise().rethrow_if_exception();
                return { nullptr };
            }
        }

        return { handle_ };
    }

    iterator end() {
        recipe_12_common::print_debug_info("generator::end");
        return { nullptr };
    }

private:
    std::coroutine_handle<promise_type> handle_ = nullptr;
};

generator<int> get_values() noexcept
{
    co_yield 1;
    co_yield 2;
    co_yield 3;
}

generator<int> iota(int start = 1, int step = 1) noexcept
{
    auto value = start;
    for (int i = 0; ; ++i) {
        co_yield value;
        value += step;
    }
}

bool is_valid_number(int n) noexcept
{
    return n % 5 != 0;
}

generator<std::optional<int>> iota_n(int start = 1, int step = 1,
        int n = std::numeric_limits<int>::max()) noexcept
{
    auto value = start;
    for (int i = 0; i < n; ++i) {
        if (is_valid_number(value)) {
            co_yield value;
        } else {
            co_yield std::nullopt;
        }
        value += step;
    }
}

generator<int> fibonacci() noexcept
{
    int a = 0;
    int b = 1;
    while (true) {
        co_yield a;
        auto tmp = a;
        a = b;
        b += tmp;
    }
}

void execute()
{
    // generate data using co_yield
    {
        for (auto i : get_values()) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }

    // generate infinite data
    {
        for (auto i : iota()) {
            std::cout << i << " ";
            if (i >= 10) {
                break;
            }
        }
        std::cout << std::endl;
    }

    // generate finite data in form of std::optional
    {
        for (auto i : iota_n(1, 1, 10)) {
            if (i.has_value()) {
                std::cout << i.value() << " ";
            }
        }
        std::cout << std::endl;
    }

    // generate infinite fibonacci data
    int counter = 1;
    for (auto i : fibonacci()) {
        std::cout << i << " ";
        if (++counter > 10) {
            break;
        }
    }
    std::cout << std::endl;
}

};
