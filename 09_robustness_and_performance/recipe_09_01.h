#pragma once

#include "recipe_09_common.h"

namespace recipe_09_01
{

void throwing_func()
{
    throw std::runtime_error("timed out");
}

void another_throwing_func()
{
    throw std::system_error(std::make_error_code(std::errc::timed_out));
}

void handle_exception()
{
    try {
        throw;  // throw current exception
    }
    catch (const std::logic_error &ex) {
        std::cout << ex.what() << std::endl;
    }
    catch (const std::runtime_error &ex) {
        std::cout << ex.what() << std::endl;
    }
    catch (const std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }
    catch (...) {
        std::cout << "unknown exception" << std::endl;
    }
}

int func1(int type, int &result)
{
    if (type < 1) {
        return 1;
    }

    // do something

    if (type > 2) {
        return 2;
    }

    // do something more

    result = 42;
    return 0;
}

enum class error_codes
{
    success,
    error_1,
    error_2,
};

error_codes func2(int type, int &result)
{
    if (type < 1) {
        return error_codes::error_1;
    }

    // do something

    if (type > 2) {
        return error_codes::error_2;
    }

    // do something more

    result = 42;
    return error_codes::success;
}

bool func3(int type, int &result)
{
    if (type < 1) {
        return false;
    }

    // do something

    if (type > 2) {
        return false;
    }

    // do something more

    result = 42;
    return true;
}

std::optional<int> func4(int type)
{
    if (type < 1) {
        return {};
    }

    // do something

    if (type > 2) {
        return {};
    }

    // do something more

    return 42;
}

class simple_error : public std::exception
{
public:
    virtual const char *what() const noexcept override {
        return "simple exception";
    }
};

class another_logic_error : public std::logic_error
{
public:
    another_logic_error() : std::logic_error("logic exception") {
    }
};

class advanced_error : public std::runtime_error
{
    int error_code;

    std::string make_message(int const ec) {
        std::stringstream ss;
        ss << "exception with code " << ec;
        return ss.str();
    }

public:
    advanced_error(int const ec) : std::runtime_error(make_message(ec).c_str()),
            error_code(ec) {
    }

    int error() const noexcept {
        return error_code;
    }
};

void throw_simple_error()
{
    throw simple_error();
}

void throw_another_logic_error()
{
    throw another_logic_error();
}

void throw_advanced_error()
{
    throw advanced_error(42);
}

void execute()
{
    auto exprint = [] (std::exception const &ex) {
        std::cout << ex.what() << std::endl;
    };

    // throw exception by value and catch it by reference
    // (or in most cases, by constant reference)
    {
        try {
            throwing_func();
        }
        catch (std::exception const &ex) {
            std::cout << ex.what() << std::endl;
        }
        std::cout << std::endl;
    }

    // order catch statements from the most derived class to the base class of the
    // hierarchy, and use catch(...) to catch all exceptions regardless of their type
    {
        try {
            another_throwing_func();
        }
        catch (std::system_error const &ex) {
            exprint(ex);
        }
        catch (std::runtime_error const &ex) {
            exprint(ex);
        }
        catch (std::exception const &ex) {
            exprint(ex);
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
        std::cout << std::endl;
    }

    // rethrow the current exception
    {
        try {
            try {
                throwing_func();
            }
            catch (...) {
                auto ex = std::current_exception();
                std::rethrow_exception(ex);
            }
        }
        catch (std::exception const &ex) {
            exprint(ex);
        }
        std::cout << std::endl;
    }

    // throw the exception object to hide the original location of the exception
    {
        try {
            throwing_func();
        }
        catch (...) {
            handle_exception();
        }
        std::cout << std::endl;
    }

    // four ways to indicate the success or failure of a function's execution
    {
        int result = -1;
        int type = 4;
        std::cout << func1(type, result) << ", " << result << std::endl;
        std::cout << static_cast<int>(func2(type, result)) << ", " <<
                result << std::endl;
        std::cout << std::boolalpha << func3(type, result) << ", " <<
                result << std::endl;
        auto opt = func4(type);
        if (opt) {
            std::cout << opt.value() << std::endl;
        } else {
            std::cout << "error, " << result << std::endl;
        }
        std::cout << std::endl;
    }

    // override virtual method what() when deriving from std::exception
    {
        try {
            throw_simple_error();
        }
        catch (simple_error const &ex) {
            exprint(ex);
        }
    }

    // provide a static description that does not depend on runtime data and pass it to
    // base class constructor when deriving from std::logic_error or std::runtime_error
    {
        try {
            throw_another_logic_error();
        }
        catch (another_logic_error const &ex) {
            exprint(ex);
        }
    }

    // provide a constructor with parameters and use them to build description message that
    // depends on runtime data when deriving from std::logic_error or std::runtime_error
    {
        try {
            throw_advanced_error();
        }
        catch (advanced_error const &ex) {
            exprint(ex);
        }
    }
}

}
