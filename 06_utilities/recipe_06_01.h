#pragma once

#include "recipe_06_common.h"

namespace recipe_06_01
{

using namespace std::chrono_literals;

void execute()
{
    // type definitions based on std::chrono::duration
    {
        std::chrono::hours half_day(12);
        std::cout << half_day.count() << std::endl;

        std::chrono::minutes half_hour(30);
        std::cout << half_hour.count() << std::endl;

        std::chrono::seconds half_minute(30);
        std::cout << half_minute.count() << std::endl;

        std::chrono::milliseconds half_second(500);
        std::cout << half_second.count() << std::endl;

        std::chrono::microseconds half_milisecond(500);
        std::cout << half_milisecond.count() << std::endl;

        std::chrono::nanoseconds half_microsecond(500);
        std::cout << half_microsecond.count() << std::endl;
        std::cout << std::endl;
    }

    // standard user-defined literal operators from C++14
    {
        auto half_day = 12h;
        std::cout << half_day.count() << std::endl;

        auto half_hour = 30min;
        std::cout << half_hour.count() << std::endl;

        auto half_minute = 30s;
        std::cout << half_minute.count() << std::endl;

        auto half_second = 500ms;
        std::cout << half_second.count() << std::endl;

        auto half_milisecond = 500us;
        std::cout << half_milisecond.count() << std::endl;

        auto half_microsecond = 500ns;
        std::cout << half_microsecond.count() << std::endl;
        std::cout << std::endl;
    }

    // direct conversion of duration from lower precision to higher precision
    {
        std::chrono::hours half_day_in_h(12);
        std::cout << half_day_in_h.count() << "h" << std::endl;

        std::chrono::minutes half_day_in_min(half_day_in_h);
        std::cout << half_day_in_min.count() << "min" << std::endl;
        std::cout << std::endl;
    }

    // use std::chrono::duration_cast to convert from higher precision to lower precision
    {
        auto secs = 12345s;
        auto hours = std::chrono::duration_cast<std::chrono::hours>(secs);
        auto minutes = std::chrono::duration_cast<std::chrono::minutes>(secs % 1h);
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(secs % 1min);

        std::cout << hours.count() << ':' << minutes.count() << ':' <<
                seconds.count() << std::endl;
        std::cout << std::endl;
    }

    // use conversion functions from C++17 when rounding is necessary
    {
        auto secs = 12345s;
        auto m1 = std::chrono::floor<std::chrono::minutes>(secs);
        auto m2 = std::chrono::round<std::chrono::minutes>(secs);
        auto m3 = std::chrono::ceil<std::chrono::minutes>(secs);
        std::cout << m1.count() << ", " << m2.count() << ", " <<
                m3.count() << std::endl;

        auto sa = std::chrono::abs(secs);
        std::cout << sa.count() << std::endl;
        std::cout << std::endl;
    }

    // modify and compare durations
    {
        auto d1 = 1h + 23min + 45s;
        auto d2 = d1 * 2;
        auto d3 = d2 - 10s;

        std::cout << d1.count() << ", " << d2.count() << ", " <<
                d3.count() <<std::endl;
        if (d1 < d2) {
            std::cout << "d1 < d2" << std::endl;
        }
        if (d2 > d3) {
            std::cout << "d2 > d3" << std::endl;
        }
        std::cout << std::endl;
    }

    // express durations with its flexible definition
    {
        std::chrono::duration<double, std::ratio<10>> d(1.2);
        std::cout << d.count() << std::endl;
        assert(std::chrono::duration_cast<std::chrono::seconds>(d).count() ==
                (12s).count());
    }
}

}
