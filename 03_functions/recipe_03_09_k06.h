#pragma once

#include "recipe_03_09_common.h"

namespace recipe_03_09_k06
{

using namespace recipe_03_09_common;

template <typename Ret, typename ...Args>
class function_base
{
public:
    virtual ~function_base() = default;
    virtual Ret call(Args... args) = 0;
    virtual function_base<Ret, Args...> *clone() = 0;
};

template <typename T, typename Ret, typename ...Args>
class function_impl : public function_base<Ret, Args...>
{
private:
    T callable_;

public:
    explicit function_impl(T &&callable)
        : callable_(std::forward<T>(callable)) {
    }

    Ret call(Args... args) override {
        return callable_(std::forward<Args>(args)...);
    }

    function_base<Ret, Args...> *clone() override {
        return new function_impl<T, Ret, Args...>(std::forward<T>(callable_));
    }
};

template <typename Signature>
class function_wrapper;

template <typename Ret, typename ...Args>
class function_wrapper<Ret(Args...)>
{
private:
    std::unique_ptr<function_base<Ret, Args...>> impl_;

public:
    function_wrapper() = default;

    function_wrapper(function_wrapper &&other) noexcept = default;

    function_wrapper(function_wrapper const &other)
        : impl_(other.impl_ ? other.impl_->clone() : nullptr) {
    }

    function_wrapper &operator =(function_wrapper const &other) {
        if (this != &other) {
            impl_ = other.impl_ ? other.impl_->clone() : nullptr;
        }
        return *this;
    }

    template <typename T>
    function_wrapper(T &&callable)
        : impl_(std::make_unique<function_impl<T, Ret,
                Args...>>(std::forward<T>(callable))) {
    }

    explicit operator bool() const {
        return impl_ != nullptr;
    }

    Ret operator ()(Args... args) {
        if (!impl_) {
            throw std::bad_function_call();
        }
        return impl_->call(std::forward<Args>(args)...);
    }
};

template <typename F, typename ...Args>
auto function_caller(F &&f, Args &&...args)
        -> decltype(std::forward<F>(f)(std::forward<Args>(args)...))
{
    return std::forward<F>(f)(std::forward<Args>(args)...);
}

template <typename C, typename T, typename ...Args>
auto function_caller(T C::*member, C &obj, Args &&...args)
        -> decltype((obj.*member)(std::forward<Args>(args)...))
{
    return (obj.*member)(std::forward<Args>(args)...);
}

template <typename C, typename T, typename ...Args>
auto function_caller(T C::*member, C *obj, Args &&...args)
        -> decltype((obj->*member)(std::forward<Args>(args)...))
{
    return (obj->*member)(std::forward<Args>(args)...);
}

template <typename C, typename T>
auto function_caller(T C::*member, C *obj)
        -> decltype(obj->*member)
{
    return obj->*member;
}

unsigned long call_func_pointer(int(*fp)(int, int), int loop)
{
    unsigned long res = 0;
    for (int i = 0; i < loop; ++i) {
        res += fp(i, i + 1);
    }
    return res;
}

unsigned long call_std_function(std::function<int(int, int)> func, int loop)
{
    unsigned long res = 0;
    for (int i = 0; i < loop; ++i) {
        res += func(i, i + 1);
    }
    return res;
}

void execute()
{
    std::array ar = { 3, 2, 7, 4, 9, 1, 5, 8, 6 };
    std::vector<int> nums;
    nums.assign(ar.begin(), ar.end());

    // use std::function
    {
        std::function<int(int, int)> func1 = add;
        std::cout << func1(2, 3) << std::endl;

        int (*fp)(int, int) = &add;
        std::function<int(int, int)> func2 = fp;
        std::cout << func2(2, 3) << std::endl;

        std::function<int(int, int)> func3 = std::plus<>();
        std::cout << func3(2, 3) << std::endl;

        std::function<bool(int)> func4 = IsOdd();
        std::cout << std::boolalpha << func4(3) << std::endl;

        std::function<bool(int)> func5 = std::bind(IntCompare(),
                5, std::placeholders::_1);
        int count = std::count_if(nums.begin(), nums.end(), func5);
        std::cout << count << std::endl;

        std::function<bool(int)> func6 = std::bind(IntCompare(),
                std::placeholders::_1, 5);
        count = std::count_if(nums.begin(), nums.end(), func6);
        std::cout << count << std::endl;

        std::function<bool(int)> func7 =
                std::not_fn(std::bind(std::modulus<int>(), std::placeholders::_1, 3));
        count = std::count_if(nums.begin(), nums.end(), func7);
        std::cout << count << std::endl;

        Student alice("Alice", 85);
        std::function<int(int)> func8 =
                std::bind(&Student::AddScore, &alice, std::placeholders::_1);
        std::cout << func8(10) << std::endl;

        std::function<int(int, int)> func9 = [] (int a, int b) {
            return a + b;
        };
        std::cout << func9(2, 3) << std::endl;
        std::cout << std::endl;
    }

    // use std::invoke
    {
        std::cout << std::invoke(add, 2, 3) << std::endl;

        int (*fp)(int, int) = &add;
        std::cout << std::invoke(fp, 2, 3) << std::endl;

        auto s = std::invoke(std::plus<>(), 2, 3);
        std::cout << s << std::endl;

        std::cout << std::boolalpha << std::invoke(IsOdd(), 3) << std::endl;

        int count = std::count_if(nums.begin(), nums.end(), [] (int x) {
            return std::invoke(std::bind(IntCompare(), 5, x));
        });
        std::cout << count << std::endl;

        count = std::count_if(nums.begin(), nums.end(), [] (int x) {
            return std::invoke(std::bind(IntCompare(), x, 5));
        });
        std::cout << count << std::endl;

        count = std::count_if(nums.begin(), nums.end(), [] (int x) {
            return std::invoke(std::not_fn(std::bind(std::modulus<int>(), x, 3)));
        });
        std::cout << count << std::endl;

        Student alice("Alice", 85);
        std::cout << std::invoke(&Student::AddScore, &alice, 10) << std::endl;

        std::cout << std::invoke([] (int a, int b) {
            return a + b;
        }, 2, 3) << std::endl;
        std::cout << std::endl;
    }

    // a possible implementation of std::function
    {
        function_wrapper<int(int, int)> func1 = &add;
        std::cout << func1(2, 3) << std::endl;

        function_wrapper<int(int, int)> func2 = IntAdd();
        std::cout << func2(2, 3) << std::endl;

        function_wrapper<int(int, int)> func3 = [] (int a, int b) {
            return a + b;
        };
        std::cout << func3(2, 3) << std::endl;

        Student alice("Alice", 85);
        function_wrapper<int(int)> func4 =
                std::bind(&Student::AddScore, alice, std::placeholders::_1);
        std::cout << func4(10) << std::endl;

        Student bob("Bob", 92);
        function_wrapper<int(int)> func5 =
                std::bind(&Student::AddScore, &bob, std::placeholders::_1);
        std::cout << func5(10) << std::endl;

        foo f;
        f.increment(3);
        function_wrapper<int()> func6 = std::bind(&foo::x, &f);
        std::cout << func6() << std::endl;
        std::cout << std::endl;
    }

    // a possible implementation of std::invoke
    {
        std::cout << function_caller(add, 2, 3) << std::endl;

        std::cout << function_caller(IntAdd(), 2, 3) << std::endl;

        std::cout << function_caller([] (int a, int b) {
            return a + b;
        }, 2, 3) << std::endl;

        Student alice("Alice", 85);
        std::cout << function_caller(&Student::AddScore, alice, 10) << std::endl;

        Student bob("Bob", 92);
        std::cout << function_caller(&Student::AddScore, &bob, 10) << std::endl;

        foo f;
        f.increment(3);
        std::cout << function_caller(&foo::x, &f) << std::endl;
        std::cout << std::endl;
    }

    // performance comparison
    {
        auto t1 = recipe_common::perf_timer<std::chrono::milliseconds>::duration(
            call_func_pointer, &add, 10000000);
        std::cout << t1 << std::endl;
        auto t2 = recipe_common::perf_timer<std::chrono::milliseconds>::duration(
            call_std_function, &add, 10000000);
        std::cout << t2 << std::endl;
    }
}

}
