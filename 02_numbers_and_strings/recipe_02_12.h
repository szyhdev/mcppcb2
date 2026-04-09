#pragma once

#include "recipe_02_common.h"

namespace recipe_02_12
{

namespace v1
{

std::string get_filename(std::string const &str)
{
    auto const pos1 { str.find_last_of('\\') };
    auto const pos2 { str.find_last_of('.') };
    return str.substr(pos1 + 1, pos2 - pos1 - 1);
}

}

namespace v2
{

std::string_view get_filename(std::string_view str)
{
    auto const pos1 { str.find_last_of('\\') };
    auto const pos2 { str.find_last_of('.') };
    return str.substr(pos1 + 1, pos2 - pos1 - 1);
}

}

std::string_view trim_view(std::string_view str)
{
    auto const pos1 { str.find_first_not_of(" ") };
    auto const pos2 { str.find_last_not_of(" ") };
    str.remove_suffix(str.length() - pos2 - 1);
    str.remove_prefix(pos1);

    return str;
}

void execute()
{
    {
        auto name1 = v1::get_filename(R"(c:\test\example1.doc)");
        std::cout << name1 << std::endl;

        auto name2 = v1::get_filename(R"(c:\test\example2)");
        std::cout << name2 << std::endl;

        if (v1::get_filename(R"(c:\test\_sample_.tmp)").front() == '_') {
            std::cout << "underscore found" << std::endl;
        }
        std::cout << std::endl;
    }

    {
        auto name1 = v2::get_filename(R"(c:\test\example1.doc)");
        std::cout << name1 << std::endl;

        auto name2 = v2::get_filename(R"(c:\test\example2)");
        std::cout << name2 << std::endl;

        if (v2::get_filename(R"(c:\test\_sample_.tmp)").front() == '_') {
            std::cout << "underscore found" << std::endl;
        }
        std::cout << std::endl;
    }

    {
        auto ts = recipe_common::perf_timer<>::duration([] () {
            for (int i = 0; i < 10000000; ++i) {
                auto name = v1::get_filename(R"(c:\test\example1.doc)");
            }
        });

        auto tsv = recipe_common::perf_timer<>::duration([] () {
            for (int i = 0; i < 10000000; ++i) {
                auto name = v2::get_filename(R"(c:\test\example1.doc)");
            }
        });

        std::cout << std::fixed << std::setprecision(0) <<
                std::chrono::duration<double, std::micro>(ts).count() << ", " <<
                std::chrono::duration<double, std::micro>(tsv).count() << std::endl;
        std::cout << std::endl;
    }

    {
        auto sv1 { trim_view("sample") };
        auto sv2 { trim_view("  sample") };
        auto sv3 { trim_view("sample  ") };
        auto sv4 { trim_view("  sample  ") };

        std::string s1 { sv1 };
        std::cout << s1 << std::endl;

        std::string s2 { sv2 };
        std::cout << s2 << std::endl;

        std::string s3 { sv3 };
        std::cout << s3 << std::endl;

        std::string s4 { sv4 };
        std::cout << s4 << std::endl;
        std::cout << std::endl;
    }

    {
        std::string s = "I am a string";
        std::cout << (void *)(s.data()) << std::endl;

        std::string s1 = s;
        std::cout << (void *)(s1.data()) << std::endl;

        std::string s2;
        s2 = s;
        std::cout << (void *)(s2.data()) << std::endl;

        std::string s3;
        s3 += s;
        std::cout << (void *)(s3.data()) << std::endl;

        std::string s4;
        s4.assign(s);
        std::cout << (void *)(s4.data()) << std::endl;

        std::string s5;
        s5.append(s);
        std::cout << (void *)(s5.data()) << std::endl;
        std::cout << std::endl;
    }

    {
        std::string_view sv { "demo" };
        std::cout << sv << std::endl;

        std::string s { sv };
        // std::string s = sv;  // error
        std::cout << s << std::endl;
    }
}

}
