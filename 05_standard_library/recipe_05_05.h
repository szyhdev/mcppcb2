#pragma once

#include "recipe_05_common.h"

namespace recipe_05_05
{

using namespace std::string_literals;

inline bool is_prime(int const number)
{
    if (((!(number & 1)) && number != 2) || (number < 2) ||
            (number % 3 == 0 && number != 3)) {
        return false;
    }

    for (int k = 1; 36 * k * k - 12 * k < number; ++k) {
        if ((number % (6 * k + 1) == 0) ||
                (number % (6 * k - 1) == 0)) {
            return false;
        }
    }

    return true;
}

void execute()
{
    std::vector<int> v { 0, 1, 1, 2, 3, 5, 8, 13 };

    auto text = "The quick brown fox jumps over the lazy dog"s;
    auto word = "over"s;

    // find a value in a range
    {
        auto itr = std::find(v.cbegin(), v.cend(), 3);
        if (itr != v.cend()) {
            std::cout << *itr << " found at [" <<
                    std::distance(v.cbegin(), itr) << "]" << std::endl;
        }
    }

    // find a value in a range that meets a criterion from a unary predicate
    {
        auto itr = std::find_if(v.cbegin(), v.cend(),
                [] (int const n) {
                    return n > 10;
                });
        if (itr != v.cend()) {
            std::cout << *itr << " found at [" <<
                    std::distance(v.cbegin(), itr) << "]" << std::endl;
        }
    }

    // find a value in a range that does not meet a criterion from a unary predicate
    {
        auto itr = std::find_if_not(v.cbegin(), v.cend(),
                [] (int const n) {
                    return n % 2 == 1;
                });
        if (itr != v.cend()) {
            std::cout << *itr << " found at [" <<
                    std::distance(v.cbegin(), itr) << "]" << std::endl;
        }
    }

    // search for occurrence of any value from a range in another range
    {
        std::vector<int> p { 7, 5, 11 };

        auto itr = std::find_first_of(v.cbegin(), v.cend(),
                p.cbegin(), p.cend());
        if (itr != v.cend()) {
            std::cout << *itr << " found at [" <<
                    std::distance(v.cbegin(), itr) << "]" << std::endl;
        }
        std::cout << std::endl;
    }

    // find last occurrence of a subrange in a range
    {
        std::vector<int> v1 { 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1 };
        std::vector<int> v2 { 1, 0, 1 };

        auto itr = std::find_end(v1.cbegin(), v1.cend(),
                v2.cbegin(), v2.cend());
        if (itr != v1.cend()) {
            std::cout << "{ 1, 0, 1 } found at [" <<
                    std::distance(v1.cbegin(), itr) << "]" << std::endl;
        }
    }

    // search for first occurrence of a subrange in a range
    {
        auto itr = std::search(text.cbegin(), text.cend(),
                word.cbegin(), word.cend());
        if (itr != text.cend()) {
            std::cout << std::quoted(word) << " found at [" <<
                    std::distance(text.cbegin(), itr) << "]" << std::endl;
        }
    }

#ifdef SEARCHERS_AVAILABLE
    // search for first occurrence of a subrange in a range with a searcher
    {
        auto itr = std::search(text.cbegin(), text.cend(),
                std::boyer_moore_searcher(word.cbegin(), word.cend()));

        if (itr != text.cend()) {
            std::cout << std::quoted(word) << " found at [" <<
                    std::distance(text.cbegin(), itr) <<
                    "] with std::boyer_moore_searcher" << std::endl;
        }
    }
#endif

    // search for N consecutive occurrences of a value in a range
    {
        std::vector<int> v1 { 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1 };

        auto itr = std::search_n(v1.cbegin(), v1.cend(), 2, 0);
        if (itr != v1.cend()) {
            std::cout << "{ 0, 0 } found at [" <<
                    std::distance(v1.cbegin(), itr) << "]" << std::endl;
        }
    }

    // find two adjacent elements in a range that are equal or
    // satisfy a binary predicate
    {
        auto itr = std::adjacent_find(v.cbegin(), v.cend());
        if (itr != v.cend()) {
            std::cout << "two adjacent equal numbers found at [" <<
                    std::distance(v.cbegin(), itr) << "]" << std::endl;
        }

        itr = std::adjacent_find(v.cbegin(), v.cend(),
                [] (int const a, int const b) {
                    return is_prime(a) && is_prime(b);
                });
        if (itr != v.cend()) {
            std::cout << "two adjacent prime numbers found at [" <<
                    std::distance(v.cbegin(), itr) << "]" << std::endl;
        }
        std::cout << std::endl;
    }

    // find whether an element exists in a sorted range
    {
        auto success = std::binary_search(v.cbegin(), v.cend(), 8);
        if (success) {
            std::cout << "8 found" << std::endl;
        }
        std::cout << std::endl;
    }

    // find first element in a range not less than a specified value
    {
        auto itr = std::lower_bound(v.cbegin(), v.cend(), 1);
        if (itr != v.cend()) {
            std::cout << "lower bound of 1 at [" <<
                    std::distance(v.cbegin(), itr) << "]" << std::endl;
        }
    }

    // find first element in a range greater than a specified value
    {
        auto itr = std::upper_bound(v.cbegin(), v.cend(), 1);
        if (itr != v.cend()) {
            std::cout << "upper bound of 1 at [" <<
                    std::distance(v.cbegin(), itr) << "]" << std::endl;
        }
    }

    // find a subrange in a range whose values are equal to a specified value
    {
        auto bounds = std::equal_range(v.cbegin(), v.cend(), 1);
        std::cout << "range of 1 at [" <<
                std::distance(v.cbegin(), bounds.first) << ", " <<
                std::distance(v.cbegin(), bounds.second) << ")" << std::endl;
    }
}

}
