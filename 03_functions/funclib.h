#pragma once

#include <map>
#include <queue>
#include <numeric>

namespace funclib
{

template <typename F, typename R>
R mapf(F &&f, R r)
{
    std::transform(std::begin(r), std::end(r), std::begin(r),
            std::forward<F>(f));

    return r;
}

template <typename F, typename T, typename U>
std::map<T, U> mapf(F &&f, std::map<T, U> const &m)
{
    std::map<T, U> r;
    for (auto const kvp : m) {
        r.insert(f(kvp));
    }

    return r;
}

template <typename F, typename T>
std::queue<T> mapf(F &&f, std::queue<T> q)
{
    std::queue<T> r;
    while (!q.empty()) {
        r.push(f(q.front()));
        q.pop();
    }

    return r;
}

template <typename F, typename R, typename T>
constexpr T foldl(F &&f, R &&r, T i)
{
    return std::accumulate(std::begin(r), std::end(r),
            std::move(i), std::forward<F>(f));
}

template <typename F, typename R, typename T>
constexpr T foldr(F &&f, R &&r, T i)
{
    return std::accumulate(std::rbegin(r), std::rend(r),
            std::move(i), std::forward<F>(f));
}

template <typename F, typename T>
constexpr T foldl(F &&f, std::queue<T> q, T i)
{
    while (!q.empty()) {
        i = f(i, q.front());
        q.pop();
    }

    return i;
}

template <typename F, typename T>
constexpr T foldr(F &&f, std::queue<T> q, T i)
{
    std::vector<int> v;
    while (!q.empty()) {
        v.push_back(q.front());
        q.pop();
    }
    std::reverse(v.begin(), v.end());

    return foldl(std::forward<F>(f), v, i);
}

template <typename F, typename T1, typename T2>
auto foldl_variadic(F &&f, T1 arg1, T2 arg2)
{
    return f(arg1, arg2);
}

template <typename F, typename T, typename ...Ts>
auto foldl_variadic(F &&f, T head, Ts ...rest)
{
    return f(head, foldl_variadic(std::forward<F>(f), rest...));
}

template <typename F, typename T1, typename T2>
auto foldr_variadic(F &&f, T1 arg1, T2 arg2)
{
    return f(arg2, arg1);
}

template <typename F, typename T, typename ...Ts>
auto foldr_variadic(F &&f, T head, Ts ...rest)
{
    return f(foldr_variadic(std::forward<F>(f), rest...), head);
}

}
