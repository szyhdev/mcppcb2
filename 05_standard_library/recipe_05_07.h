#pragma once

#include "recipe_05_common.h"

namespace recipe_05_07
{

struct date
{
    int year;
    int month;
    int day;

    date() = default;

    date(int y, int m, int d) : year(y), month(m), day(d) {
    }

    date &operator =(const date &) = default;

    date &operator ++() {
        int max_day;
        ++day;
        if ((month == 1) || (month == 3) || (month == 5) || (month == 7) ||
                (month == 8) || (month == 10) || (month == 12)) {
            max_day = 31;
        } else if ((month == 4) || (month == 6) || (month == 9) || (month == 11)) {
            max_day = 30;
        } else if (month == 2) {
            bool is_leap_year = ((year % 4 == 0) && (year % 100 != 0)) ||
                    (year % 400 == 0);
            max_day = is_leap_year ? 29 : 28;
        } else {
            assert(0);
            day--;
            return *this;
        }

        if (day > max_day) {
            day = 1;
            ++month;
            if (month > 12) {
                month = 1;
                ++year;
            }
        }

        return *this;
    }

    std::string to_string() const {
        std::ostringstream oss;
        oss << year << "-" << std::setw(2) << std::setfill('0') << month
            << "-" << std::setw(2) << std::setfill('0') << day;
        return oss.str();
    }
};

void execute()
{
    // assign a value to all elements of a range
    {
        std::vector<int> v(5);
        std::fill(v.begin(), v.end(), 42);
        recipe_common::print_collection(v, "v: ");

        std::array<int, 5> a;
        std::fill(a.begin(), a.end(), 42);
        recipe_common::print_collection(a, "a: ");
    }

    // assign values to a number of elements of a range
    {
        std::vector<int> v(10);
        std::fill_n(v.begin(), 5, 42);
        recipe_common::print_collection(v, "v: ");
    }

    // assign value returned by a function to elements of a range
    {
        std::random_device rd {};
        std::mt19937 mt { rd() };
        std::uniform_int_distribution<> ud { 1, 10 };

        std::vector<int> v(5);
        std::generate(v.begin(), v.end(), [&ud, &mt] {
            return ud(mt);
        });
        recipe_common::print_collection(v, "v: ");
    }

    // assign value returned by a function to a number of elements of a range
    {
        std::vector<int> v(5);
        auto i = 1;
        std::generate_n(v.begin(), v.size(), [&i] {
            return i * i++;
        });
        recipe_common::print_collection(v, "v: ");
    }

    // assign sequentially increasing values to elements of a range
    {
        std::vector<int> v(5);
        std::iota(v.begin(), v.end(), 1);
        recipe_common::print_collection(v, "v: ");
    }

    // assign sequentially increasing values to elements of a range
    // with a user-defined type
    {
        std::vector<date> dates(5);
        std::iota(dates.begin(), dates.end(), date(2024, 2, 26));

        for (auto const &d : dates) {
            std::cout << d.to_string() << " ";
        }
        std::cout << std::endl;
    }
}

}
