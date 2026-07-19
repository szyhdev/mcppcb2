#pragma once

#include "recipe_12_common.h"

// #define USE_RANGE_V3

#ifdef USE_RANGE_V3

#include "range/v3/algorithm/sort.hpp"
#include "range/v3/algorithm/copy.hpp"
#include "range/v3/algorithm/reverse.hpp"
#include "range/v3/algorithm/count_if.hpp"
#include "range/v3/algorithm/max.hpp"
#include "range/v3/iterator/stream_iterators.hpp"
#include "range/v3/view.hpp"

namespace rv = ranges::views;
namespace rg = ranges;

#define OSTREAM_ITERATOR ranges::ostream_iterator

#else

namespace rv = std::ranges::views;
namespace rg = std::ranges;

#define OSTREAM_ITERATOR std::ostream_iterator

#endif

namespace recipe_12_06
{

template <std::ranges::viewable_range R>
constexpr auto my_algo(R &&r)
{
    using Iter = std::ranges::borrowed_iterator_t<R>;

    if constexpr (std::same_as<Iter, std::ranges::dangling>) {
        return std::ranges::dangling {};
    } else {
        return std::begin(r);
    }
}

template <rg::bidirectional_range R, typename P>
requires rg::viewable_range<R>
class trim_view : public rg::view_interface<trim_view<R, P>>
{
private:
    R base_;
    P pred_;
    mutable rg::iterator_t<R> begin_ { std::begin(base_) };
    mutable rg::iterator_t<R> end_ { std::end(base_) };
    mutable bool evaluated_ = false;

private:
    void ensure_evaluated() const {
        if (!evaluated_) {
            while (begin_ != std::end(base_) && pred_(*begin_)) {
                begin_ = std::next(begin_);
            }
            while (end_ != begin_ && pred_(*std::prev(end_))) {
                end_ = std::prev(end_);
            }
            evaluated_ = true;
        }
    }

public:
    trim_view() = default;

    constexpr trim_view(R base, P pred) :
            base_(std::move(base)), pred_(std::move(pred)),
            begin_(std::begin(base_)), end_(std::end(base_)) {
    }

    constexpr R base() const & {
        return base_;
    }

    constexpr R base() && {
        return std::move(base_);
    }

    constexpr P const &pred() const {
        return pred_;
    }

    constexpr auto begin() const {
        ensure_evaluated();
        return begin_;
    }

    constexpr auto end() const {
        ensure_evaluated();
        return end_;
    }

    constexpr auto size()
    requires rg::sized_range<R> {
        return std::distance(begin_, end_);
    }

    constexpr auto size() const
    requires rg::sized_range<const R> {
        return std::distance(begin_, end_);
    }
};

template <class R, typename P>
trim_view(R &&base, P pred) -> trim_view<rg::views::all_t<R>, P>;

namespace details
{

template <typename P>
struct trim_view_range_adaptor_closure
{
    P pred_;

    constexpr trim_view_range_adaptor_closure(P pred) : pred_(pred) {
    }

    template <rg::viewable_range R>
    constexpr auto operator ()(R &&r) const {
        return trim_view(std::forward<R>(r), pred_);
    }
};

struct trim_view_range_adaptor
{
    template <typename P>
    constexpr auto operator ()(P pred) {
        return trim_view_range_adaptor_closure(pred);
    }

    template <rg::viewable_range R, typename P>
    constexpr auto operator ()(R &&r, P pred) {
        return trim_view(std::forward<R>(r), pred);
    }
};

template <rg::viewable_range R, typename P>
constexpr auto operator |(R &&r, trim_view_range_adaptor_closure<P> const &a)
{
    return a(std::forward<R>(r));
}

}

namespace views
{
    inline static details::trim_view_range_adaptor trim;
}

template <typename P>
void are_equal(std::vector<int> const &input,
        std::vector<int> const &output, P &&pred)
{
    std::size_t index = 0;
    for (auto i : input | views::trim(std::forward<P>(pred))) {
        assert(i == output[index]);
        index++;
    }
    assert(index == output.size());
}

template <typename P>
void are_equal_reverse1(std::vector<int> const &input,
        std::vector<int> const &output, P &&pred)
{
    std::size_t index = 0;
    for (auto i : input | rg::views::reverse | views::trim(std::forward<P>(pred))) {
        assert(i == output[index]);
        index++;
    }
    assert(index == output.size());
}

template <typename P>
void are_equal_reverse2(std::vector<int> const &input,
        std::vector<int> const &output, P &&pred)
{
    std::size_t index = 0;
    for (auto i : input | views::trim(std::forward<P>(pred)) | rg::views::reverse) {
        assert(i == output[index]);
        index++;
    }
    assert(index == output.size());
}

void execute()
{
    // use std::ranges::borrowed_range
    {
        std::vector<int> vec = { 1, 2, 3, 4, 5 };
        auto itr1 = std::ranges::find(vec, 3);
        if (itr1 != vec.end()) {
            std::cout << *itr1 << " found" << std::endl;
        }

        std::span<int> sp = vec;
        auto itr2 = std::ranges::find(std::span { vec }, 4);
        if (itr2 != std::ranges::end(sp)) {
            std::cout << *itr2 << " found" << std::endl;
        }

        auto itr3 = std::ranges::find(std::vector<int> { 1, 2, 3, 4, 5 }, 2);
        // std::cout << *itr3 << " found" << std::endl;  // compile error
        std::cout << std::endl;
    }

    // use std::ranges::viewable_range
    {
        std::vector<int> vec = { 1, 2, 3, 4, 5 };
        auto v1 = vec | std::ranges::views::filter([] (int i) { return i % 2 == 0; });
        std::ranges::copy(v1, OSTREAM_ITERATOR<int>(std::cout, " "));
        std::cout << std::endl;

        auto v2 = std::ranges::views::iota(1, 5) | std::ranges::views::take(3);
        std::ranges::copy(v2, OSTREAM_ITERATOR<int>(std::cout, " "));
        std::cout << std::endl;

        auto v3 = std::vector<int> { 1, 2, 3, 4, 5 } | std::ranges::views::take(3);
        // warning: implementation dependent

        auto il = { 1, 2, 3, 4, 5 };
        auto v4 = il | std::ranges::views::take(3);
        // warning: implementation dependent
        std::cout << std::endl;
    }

    // use std::ranges::dangling
    {
        auto itr1 = my_algo(std::string_view { "Hello, C++20 Ranges"});
        std::cout << *itr1 << std::endl;

        auto itr2 = my_algo(std::vector<int> { 1, 2, 3, 4, 5 });
        // std::cout << *itr2 << std::endl;  // compile error
        std::cout << std::endl;
    }

    std::vector<int> n { 1, 1, 2, 3, 4, 5, 6, 7, 7, 9 };

    auto is_odd = [] (int const n) {
        return n % 2 == 1;
    };

    // use trim_view with other views by directly constructing it
    {
        auto v1 = trim_view(n, is_odd);
        rg::copy(v1, OSTREAM_ITERATOR<int>(std::cout, " "));
        std::cout << std::endl;

        auto v2 = views::trim(n, is_odd);
        rg::copy(v2, OSTREAM_ITERATOR<int>(std::cout, " "));
        std::cout << std::endl;

        for (auto i : rv::reverse(trim_view(n, is_odd))) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }

    // use trim_view with other views by overloaded pipe operator
    {
        auto v = n | views::trim(is_odd);
        rg::copy(v, OSTREAM_ITERATOR<int>(std::cout, " "));
        std::cout << std::endl;

        for (auto i : n | views::trim(is_odd) | rv::reverse) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }

    // test cases for trim_view
    {
        // case 1: range | trim(pred)
        are_equal({},  {}, is_odd);
        are_equal({ 1 }, {}, is_odd);
        are_equal({ 1, 3, 5 }, {}, is_odd);
        are_equal({ 2 }, { 2 }, is_odd);
        are_equal({ 2, 4 }, { 2, 4 }, is_odd);
        are_equal({ 2, 3, 4 }, { 2, 3, 4 }, is_odd);
        are_equal({ 1, 2, 3, 4 }, { 2, 3, 4 }, is_odd);
        are_equal({ 1, 1, 2, 3, 4 }, { 2, 3, 4 }, is_odd);
        are_equal({ 2, 3, 4, 5 }, { 2, 3, 4 }, is_odd);
        are_equal({ 2, 3, 4, 5, 5 }, { 2, 3, 4 }, is_odd);
        are_equal({ 1, 2, 3, 4, 5 }, { 2, 3, 4 }, is_odd);
        are_equal({ 1, 1, 2, 3, 4, 5, 5 }, { 2, 3, 4 }, is_odd);

        // case 2: range | reverse | trim(pred)
        are_equal_reverse1({}, {}, is_odd);
        are_equal_reverse1({ 1 }, {}, is_odd);
        are_equal_reverse1({ 1, 3, 5 }, {}, is_odd);
        are_equal_reverse1({ 2 }, { 2 }, is_odd);
        are_equal_reverse1({ 2, 4 }, { 4, 2 }, is_odd);
        are_equal_reverse1({ 2, 3, 4 }, { 4, 3, 2 }, is_odd);
        are_equal_reverse1({ 1, 2, 3, 4 }, { 4, 3, 2 }, is_odd);
        are_equal_reverse1({ 1, 1, 2, 3, 4 }, { 4, 3, 2 }, is_odd);
        are_equal_reverse1({ 2, 3, 4, 5 }, { 4, 3, 2 }, is_odd);
        are_equal_reverse1({ 2, 3, 4, 5, 5 }, { 4, 3, 2 }, is_odd);
        are_equal_reverse1({ 1, 2, 3, 4, 5 }, { 4, 3, 2 }, is_odd);
        are_equal_reverse1({ 1, 1, 2, 3, 4, 5, 5 }, { 4, 3, 2 }, is_odd);

        // case 3: range | trim(pred) | reverse
        are_equal_reverse2({}, {}, is_odd);
        are_equal_reverse2({ 1 }, {}, is_odd);
        are_equal_reverse2({ 1, 3, 5 }, {}, is_odd);
        are_equal_reverse2({ 2 }, { 2 }, is_odd);
        are_equal_reverse2({ 2, 4 }, { 4, 2 }, is_odd);
        are_equal_reverse2({ 2, 3, 4 }, { 4, 3, 2 }, is_odd);
        are_equal_reverse2({ 1, 2, 3, 4 }, { 4, 3, 2 }, is_odd);
        are_equal_reverse2({ 1, 1, 2, 3, 4 }, { 4, 3, 2 }, is_odd);
        are_equal_reverse2({ 2, 3, 4, 5 }, { 4, 3, 2 }, is_odd);
        are_equal_reverse2({ 2, 3, 4, 5, 5 }, { 4, 3, 2 }, is_odd);
        are_equal_reverse2({ 1, 2, 3, 4, 5 }, { 4, 3, 2 }, is_odd);
        are_equal_reverse2({ 1, 1, 2, 3, 4, 5, 5 }, { 4, 3, 2 }, is_odd);
    }
}

}
