#pragma once

#include "recipe_02_common.h"

namespace recipe_02_05
{

#define USE_FUNCTION_FORM
#define USE_TEMPLATE_FORM

constexpr size_t operator ""_KB(unsigned long long const value)
{
    return static_cast<size_t>(value * 1024);
}

#if defined(USE_FUNCTION_FORM)

constexpr size_t operator ""_KB(char const *str)
{
    unsigned long long value = 0;
    std::size_t len = std::strlen(str);
    for (std::size_t i = 0; i < len; ++i) {
        if ((str[i] >= '0') && (str[i] <= '9')) {
            value = value * 10 + (str[i] - '0');
        } else {
            throw std::invalid_argument("Invalid decimal digit");
        }
    }
    return static_cast<size_t>(value * 1024);
}

#elif defined(USE_TEMPLATE_FORM)

template <char... chars>
constexpr size_t operator ""_KB()
{
    unsigned long long value = 0;
    ((value = value * 10 + (char(chars) - '0')), ...);
    return static_cast<size_t>(value * 1024);
}

#endif

constexpr size_t operator ""_KB(char const *str, size_t len)
{
    unsigned long long value = 0;
    for (std::size_t i = 0; i < len; ++i) {
        if ((str[i] >= '0') && (str[i] <= '9')) {
            value = value * 10 + (str[i] - '0');
        } else {
            throw std::invalid_argument("Invalid decimal digit");
        }
    }
    return static_cast<size_t>(value * 1024);
}

namespace units
{
    enum class unit
    {
        kilogram, liter, meter, piece
    };

    template <unit U>
    class quantity
    {
        const double amount;

    public:
        constexpr explicit quantity(double const a) : amount(a) {
        }

        explicit operator double() const {
            return amount;
        }
    };

    template <unit U>
    constexpr quantity<U> operator +(quantity<U> const &q1, quantity<U> const &q2)
    {
        return quantity<U>(static_cast<double>(q1) + static_cast<double>(q2));
    }

    template <unit U>
    constexpr quantity<U> operator -(quantity<U> const &q1, quantity<U> const &q2)
    {
        return quantity<U>(static_cast<double>(q1) - static_cast<double>(q2));
    }

    template <unit U>
    std::ostream &operator <<(std::ostream &os, quantity<U> const &q)
    {
        os << (double)(q);
        return os;
    }

    namespace unit_literals
    {
        constexpr quantity<unit::kilogram> operator ""_kg(long double const amount)
        {
            return quantity<unit::kilogram> { static_cast<double>(amount) };
        }

        constexpr quantity<unit::kilogram> operator ""_kg(unsigned long long const amount)
        {
            return quantity<unit::kilogram> { static_cast<double>(amount) };
        }

        constexpr quantity<unit::liter> operator ""_l(long double const amount)
        {
            return quantity<unit::liter> { static_cast<double>(amount) };
        }

        constexpr quantity<unit::meter> operator ""_m(long double const amount)
        {
            return quantity<unit::meter> { static_cast<double>(amount) };
        }

        constexpr quantity<unit::piece> operator ""_pcs(unsigned long long const amount)
        {
            return quantity<unit::piece> { static_cast<double>(amount) };
        }
    }
}

void execute()
{
    // use literal operator ""_KB
    {
        auto size1 { 40_KB };
        std::cout << size1 << std::endl;

        using byte = unsigned char;
        auto buffer = std::array<byte, 1_KB> {};
        std::cout << buffer.size() << std::endl;

        auto size2 { "10002"_KB };
        std::cout << size2 << std::endl;

        std::cout << std::endl;
    }

    // define a set of units and quantities
    {
        using namespace units;
        using namespace unit_literals;

        auto q1 { 1_kg };
        std::cout << q1 << std::endl;
        auto q2 { 4.5_kg };
        std::cout << q2 << std::endl;
        auto q3 { q1 + q2 };
        std::cout << q3 << std::endl;
        auto q4 { q2 - q1 };
        std::cout << q4 << std::endl;

        // auto q5 { 1.0_m + 1_pcs };  // error: add meters and pieces
        // auto q6 { 1_l };            // error: have an integer number of liter
        // auto q7 { 1_m };            // error: have an integer number of meter
        // auto q8 { 2.0_pcs };        // error: have a double number of piece
        std::cout << std::endl;
    }

    // a list of standard literals
    {
        // operator ""s for creating values of different string types
        using namespace std::string_literals;

        auto s1 { "text"s };
        static_assert(std::is_same_v<decltype(s1), std::string>);
        std::cout << s1 << std::endl;
        auto s2 { L"text"s };
        static_assert(std::is_same_v<decltype(s2), std::wstring>);
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::cout << converter.to_bytes(s2) << std::endl;
        auto s3 { u"text"s };
        static_assert(std::is_same_v<decltype(s3), std::u16string>);
        std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> converter16;
        std::cout << converter16.to_bytes(s3) << std::endl;
        auto s4 { U"text"s };
        static_assert(std::is_same_v<decltype(s4), std::u32string>);
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter32;
        std::cout << converter32.to_bytes(s4) << std::endl;

        // operator ""sv for creating a std::basic_string_view value
        using namespace std::string_view_literals;

        auto s5 { "text"sv };
        static_assert(std::is_same_v<decltype(s5), std::string_view>);
        std::cout << s5 << std::endl;

        // operator ""h, operator ""min, operator ""s, operator ""ms, operator ""us,
        // and operator ""ns for creating a std::chrono::duration value
        using namespace std::chrono_literals;

        auto timer { 2h + 42min + 15s };
        static_assert(std::is_same_v<decltype(timer), std::chrono::seconds>);
        std::cout << timer.count() << "s" << std::endl;

        // operator ""y for creating a std::chrono::year value and
        // operator ""d for creating a std::chrono::day value
        using namespace std::chrono_literals;

        auto year { 2020y };
        static_assert(std::is_same_v<decltype(year), std::chrono::year>);
        std::cout << year << std::endl;
        auto day { 15d };
        static_assert(std::is_same_v<decltype(day), std::chrono::day>);
        std::cout << day << std::endl;

        // operator ""if for creating a float std::complex value,
        // operator ""i for creating a double std::complex value,
        // and operator ""il for creating a long double std::complex value
        using namespace std::complex_literals;

        auto c { 12.0 + 4.5i };
        static_assert(std::is_same_v<decltype(c), std::complex<double>>);
        std::cout << "(" << c.real() << ", " << c.imag() << ")" << std::endl;
    }
}

}
