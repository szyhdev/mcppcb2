#pragma once

#include "recipe_06_common.h"

#include <chrono>

namespace recipe_06_03
{

using namespace std::chrono_literals;

void execute()
{
#ifdef HAS_CALENDAR_AND_TIMEZONES
    // retrieve current time in local zone
    {
        auto time = std::chrono::zoned_time {
            std::chrono::current_zone(),
            std::chrono::system_clock::now()
        };
        std::cout << time << std::endl;  // 2025-10-22 22:21:35.011860339 CST
        std::cout << std::endl;
    }

    // retrieve current time in another time zone
    {
        auto time = std::chrono::zoned_time {
            std::chrono::locate_zone("Europe/Rome"),
            std::chrono::system_clock::now()
        };
        std::cout << time << std::endl;  // 2025-10-22 16:21:35.011975998 CEST
        std::cout << std::endl;
    }

    // display current local time with proper locale formatting
    {
        auto time = std::chrono::zoned_time {
            std::chrono::current_zone(),
            std::chrono::system_clock::now()
        };
        std::cout << std::format(std::locale("zh_CN.UTF-8"), "{:%c}", time) <<
                std::endl;               // Wed Oct 22 22:21:35.012106534 2025 (is it correct?)
        std::cout << std::endl;
    }

    // represent a time point in a particular time zone, and then
    // convert it into a time point in another time zone
    {
        auto time = std::chrono::local_days { 2020y / std::chrono::June / 1 } +
                12h + 30min + 45s + 256ms;
        auto ny_time = std::chrono::zoned_time<std::chrono::milliseconds> {
            std::chrono::locate_zone("America/New_York"),
            time
        };
        std::cout << ny_time << std::endl;  // 2020-06-01 12:30:45.256 EDT

        auto la_time = std::chrono::zoned_time<std::chrono::milliseconds>(
                std::chrono::locate_zone("America/Los_Angeles"),
                ny_time);
        std::cout << la_time << std::endl;  // 2020-06-01 09:30:45.256 PDT
        std::cout << std::endl;
    }

    // convert a time point in another time zone
    {
        auto zst = std::chrono::zoned_time<std::chrono::seconds>(
                std::chrono::current_zone(),
                std::chrono::sys_days { 2020y / std::chrono::May / 10 } +
                        14h + 20min + 30s);
        std::cout << zst << std::endl;  // 2020-05-10 22:20:30 CST

        auto zlt = std::chrono::zoned_time<std::chrono::seconds>(
                std::chrono::current_zone(),
                std::chrono::local_days{ 2020y / std::chrono::May / 10 } +
                        14h + 20min + 30s);
        std::cout << zlt << std::endl;  // 2020-05-10 14:20:30 CST

        auto zpt = std::chrono::zoned_time<std::chrono::seconds>(
                std::chrono::locate_zone("Europe/Paris"),
                zlt);
        std::cout << zpt << std::endl;  // 2020-05-10 08:20:30 CEST
    }
#endif
}

}
