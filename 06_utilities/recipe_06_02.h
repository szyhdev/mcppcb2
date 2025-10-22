#pragma once

#include "recipe_06_common.h"

#include <chrono>

namespace recipe_06_02
{

using namespace std::chrono_literals;

template <typename T>
using time_of_day = std::chrono::hh_mm_ss<T>;

#ifdef HAS_CALENDAR_AND_TIMEZONES
inline int number_of_days(std::chrono::sys_days const &first,
        std::chrono::sys_days const &last)
{
    return (last - first).count();
}
#endif

void execute()
{
#ifdef HAS_CALENDAR_AND_TIMEZONES
    // represent calendar dates with year, month and day
    {
        // year / month / day
        std::chrono::year_month_day d1 = 2020y / 1 / 15;
        std::cout << d1 << std::endl;
        std::chrono::year_month_day d2 = 2020y / std::chrono::January / 15;
        std::cout << d2 << std::endl;

        // day / month / year
        std::chrono::year_month_day d3 = 15d / 1 / 2020;
        std::cout << d3 << std::endl;
        std::chrono::year_month_day d4 = 15d / std::chrono::January / 2020;
        std::cout << d4 << std::endl;

        // month / day / year
        std::chrono::year_month_day d5 = 1 / 15d / 2020;
        std::cout << d5 << std::endl;
        std::chrono::year_month_day d6 = std::chrono::January / 15 / 2020;
        std::cout << d6 << std::endl;
        std::cout << std::endl;
    }

    // represent calendar dates with year, month and weekday
    {
        // year / month / weekday
        std::chrono::year_month_weekday d1 =
                2020y / std::chrono::January / std::chrono::Monday[1];
        std::cout << std::chrono::sys_days(d1) << std::endl;

        // weekday / month / year
        std::chrono::year_month_weekday d2 =
                std::chrono::Monday[1] / std::chrono::January / 2020;
        std::cout << std::chrono::sys_days(d2) << std::endl;

        // month / weekday / year
        std::chrono::year_month_weekday d3 =
                std::chrono::January / std::chrono::Monday[1] / 2020;
        std::cout << std::chrono::sys_days(d3) << std::endl;
        std::cout << std::endl;
    }

    // determine current date and relative dates
    {
        auto today = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
        std::cout << today << std::endl;

        auto tomorrow = today + std::chrono::days { 1 };
        std::cout << tomorrow << std::endl;

        auto yesterday = today - std::chrono::days { 1 };
        std::cout << yesterday << std::endl;
        std::cout << std::endl;
    }

    // determine first and last day of a specific month and year
    {
        std::chrono::year_month_day today =
                std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
        std::cout << today << std::endl;

        std::chrono::year_month_day first_day_this_month =
                today.year() / today.month() / 1;
        std::cout << first_day_this_month << std::endl;

        std::chrono::year_month_day last_day_this_month =
                today.year() / today.month() / std::chrono::last;
        std::cout << last_day_this_month << std::endl;

        std::chrono::year_month_day last_day_feb_2020 =
                2020y / std::chrono::February / std::chrono::last;
        std::cout << last_day_feb_2020 << std::endl;

        std::chrono::year_month_day_last ymdl {
            today.year(),
            std::chrono::month_day_last { std::chrono::month { 2 } }
        };
        std::chrono::year_month_day last_day_feb { ymdl };
        std::cout << last_day_feb << std::endl;
        std::cout << std::endl;
    }

    // compute the number of days between two dates
    {
        auto days = number_of_days(2020y / std::chrono::April / 1,
                2020y / std::chrono::December / 25);
        std::cout << days << std::endl;
        std::cout << std::endl;
    }

    // check whether a date is valid
    {
        auto d1 = 2020y / std::chrono::February / 29;
        std::cout << std::boolalpha << d1.ok() << std::endl;
        auto d2 = 2020y / std::chrono::January / 33;
        std::cout << d2.ok() << std::endl;
        std::cout << std::endl;
    }

    // represent the time of day with hours, minutes and seconds
    {
        time_of_day<std::chrono::seconds> tod(13h + 12min + 11s);
        std::cout << tod << std::endl;
        std::cout << std::endl;
    }

    // create time points with date and time parts
    {
        auto tp1 = std::chrono::sys_days { 2020y / std::chrono::April / 1 };
        std::cout << tp1 << std::endl;
        auto tp2 = std::chrono::sys_days { 2020y / std::chrono::April / 1 } +
                12h + 30min + 45s;
        std::cout << tp2 << std::endl;
        std::cout << std::endl;
    }

    // determine current time of day and express it with various precisions
    {
        auto tp = std::chrono::system_clock::now();
        std::cout << tp << std::endl;
        auto dp = std::chrono::floor<std::chrono::days>(tp);
        std::cout << dp << std::endl;

        time_of_day<std::chrono::milliseconds> tod1 {
            std::chrono::duration_cast<std::chrono::milliseconds>(tp - dp)
        };
        std::cout << tod1 << std::endl;

        time_of_day<std::chrono::minutes> tod2 {
            std::chrono::duration_cast<std::chrono::minutes>(tp - dp)
        };
        std::cout << tod2 << std::endl;
    }
#endif
}

}
