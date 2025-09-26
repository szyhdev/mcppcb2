#pragma once

#include <iostream>
#include <functional>

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
    std::vector<int> v { 1, 1, 2, 3, 5, 8, 13 };

    // find a value in a range
    {
        auto itr = std::find(v.cbegin(), v.cend(), 3);
        if (itr != v.cend()) {
            std::cout << *itr << std::endl;
        }
    }

    // find a value in a range that meets a criterion
    {
        auto itr = std::find_if(v.cbegin(), v.cend(),
                [] (int const n) {
                    return n > 10;
                });
        if (itr != v.cend()) {
            std::cout << *itr << std::endl;
        }
    }

    // find a value in a range that does not meet a criterion
    {
        auto itr = std::find_if_not(v.cbegin(), v.cend(),
                [] (int const n) {
                    return n % 2 == 1;
                });
        if (itr != v.cend()) {
            std::cout << *itr << std::endl;
        }
    }

    // search for occurrence of any value from a range in another range
    {
        std::vector<int> p { 5, 7, 11 };

        auto itr = std::find_first_of(v.cbegin(), v.cend(),
                p.cbegin(), p.cend());
        if (itr != v.cend()) {
            std::cout << "found " << *itr << " at index " <<
                    std::distance(v.cbegin(), itr) << std::endl;
        }
    }

    // find last occurrence of a subrange of elements in a range
    {
        std::vector<int> v1 { 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1 };
        std::vector<int> v2 { 1, 0, 1 };

        auto itr = std::find_end(v1.cbegin(), v1.cend(),
                v2.cbegin(), v2.cend());
        if (itr != v1.cend()) {
            std::cout << "found { 1, 0, 1 } at index " <<
                    std::distance(v1.cbegin(), itr) << std::endl;
        }
    }

    // search for first occurrence of a subrange in a range
    {
        auto text = "The quick brown fox jumps over the lazy dog"s;
        auto word = "over"s;

        auto itr = std::search(text.cbegin(), text.cend(),
                word.cbegin(), word.cend());
        if (itr != text.cend()) {
            std::cout << "found '" << word << "' at index " <<
                    std::distance(text.cbegin(), itr) << std::endl;
        }
    }

#ifdef SEARCHERS_AVAILABLE
    // search for first occurrence of a subrange in a range with a searcher
    {
        auto text = "The quick brown fox jumps over the lazy dog"s;
        auto word = "over"s;

        auto itr = std::search(text.cbegin(), text.cend(),
                std::boyer_moore_searcher(word.cbegin(), word.cend()));

        if (itr != text.cend()) {
            std::cout << "found '" << word << "' at index " <<
                    std::distance(text.cbegin(), itr) << std::endl;
        }
    }
#endif

    // search for N consecutive occurrences of a value in a range
    {
        std::vector<int> v { 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1 };

        auto itr = std::search_n(v.cbegin(), v.cend(), 2, 0);
        if (itr != v.cend()) {
            std::cout << "found { 0, 0 } at index " <<
                    std::distance(v.cbegin(), itr) << std::endl;
        }
    }

    // find two adjacent elements in a range that are equal or
    // satisfy a binary predicate
    {
        auto itr = std::adjacent_find(v.cbegin(), v.cend());
        if (itr != v.cend()) {
            std::cout << "found at index " <<
                    std::distance(v.cbegin(), itr) << std::endl;
        }

        itr = std::adjacent_find(v.cbegin(), v.cend(),
                [] (int const a, int const b) {
                    return is_prime(a) && is_prime(b);
                });
        if (itr != v.cend()) {
            std::cout << "found at index " <<
                    std::distance(v.cbegin(), itr) << std::endl;
        }
    }

    // find whether an element exists in a sorted range
    {
        auto success = std::binary_search(v.cbegin(), v.cend(), 8);
        if (success) {
            std::cout << "8 found" << std::endl;
        }
    }

    // find first element in a range not less than a specified value
    {
        auto itr = std::lower_bound(v.cbegin(), v.cend(), 1);
        if (itr != v.cend()) {
            std::cout << "lower bound of 1 at " <<
                        std::distance(v.cbegin(), itr) << std::endl;
        }
    }

    // find first element in a range greater than a specified value
    {
        auto itr = std::upper_bound(v.cbegin(), v.cend(), 1);
        if (itr != v.cend()) {
            std::cout << "upper bound of 1 at " <<
                    std::distance(v.cbegin(), itr) << std::endl;
        }
    }

    // find a subrange in a range whose values are equal to a specified value
    {
        auto bounds = std::equal_range(v.cbegin(), v.cend(), 1);
        std::cout << "range between indexes " <<
                std::distance(v.cbegin(), bounds.first) << " and " <<
                std::distance(v.cbegin(), bounds.second) << std::endl;
    }
}

}
