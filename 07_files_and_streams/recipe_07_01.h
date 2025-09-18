#pragma once

#include <iostream>
#include <cassert>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>

namespace recipe_07_01
{

bool write_data(char const * const filename,
        char const * const data, size_t const size)
{
    auto success = false;
    std::ofstream ofile(filename, std::ios::binary);
    if (ofile.is_open()) {
        try {
            ofile.write(data, size);
            success = true;
        }
        catch (std::ios_base::failure &) {
            // handle the error
        }
        ofile.close();
    }

    return success;
}

size_t read_data(char const * const filename,
        std::function<char *(size_t const)> allocator)
{
    size_t readbytes = 0;
    std::ifstream ifile(filename, std::ios::ate | std::ios::binary);
    if (ifile.is_open()) {
        auto length = static_cast<size_t>(ifile.tellg());
        ifile.seekg(0, std::ios_base::beg);
        auto buffer = allocator(length);
        try {
            ifile.read(buffer, length);
            readbytes = static_cast<size_t>(ifile.gcount());
        }
        catch (std::ios_base::failure &) {
            // handle the error
        }
        ifile.close();
    }

    return readbytes;
}

void execute()
{
    const char *file_name = "sample.bin";
    std::vector<unsigned char> output {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9
    };
    std::vector<unsigned char> input;

    auto check_result = [&file_name] (size_t length, bool is_equal) {
        std::cout << length << " read: " << is_equal << std::endl;
    };

    std::cout << std::boolalpha;

    // write the content of a buffer to a binary file
    {
        std::ofstream ofile(file_name, std::ios::binary);
        if (ofile.is_open()) {
            ofile.write(reinterpret_cast<char *>(output.data()),
                    output.size());
            // ofile.flush();  // optional
            assert(!ofile.fail());
            ofile.close();  // calls flush()
        }

        std::cout << output.size() << " written" << std::endl;
    }

    // read the entire content of a binary file to a buffer
    {
        input.clear();
        size_t length = 0;
        std::ifstream ifile(file_name, std::ios::binary);
        if (ifile.is_open()) {
            ifile.seekg(0, std::ios_base::end);
            length = ifile.tellg();
            ifile.seekg(0, std::ios_base::beg);

            input.resize(static_cast<size_t>(length));
            ifile.read(reinterpret_cast<char *>(input.data()), length);

            assert(!ifile.fail() && (length == ifile.gcount()));

            ifile.close();
        }

        check_result(length, (input == output));
    }

    // two general functions for writing and reading data to and from a file
    {
        input.clear();

        if (write_data(file_name, reinterpret_cast<char *>(output.data()),
                output.size())) {
            size_t readbyte = 0;
            if ((readbyte = read_data(file_name,
                    [&input] (size_t const length) {
                        input.resize(length);
                        return reinterpret_cast<char *>(input.data());
                    })) > 0) {
                check_result(readbyte, (input == output));
            }
        }
    }

    // use a dynamically allocated buffer instead of std::vector
    {
        unsigned char *input = nullptr;
        size_t readbyte = 0;
        if (write_data(file_name, reinterpret_cast<char *>(output.data()),
                output.size())) {
            if ((readbyte = read_data(file_name,
                    [&input] (size_t const length) {
                        input = new unsigned char[length];
                        return reinterpret_cast<char *>(input);
                    })) > 0) {
                check_result(readbyte, (memcmp(output.data(), input, output.size()) == 0));
            }
            delete [] input;
        }
    }

    // initialize std::vector using std::istreambuf_iterator iterators
    {
        input.clear();
        std::ifstream ifile(file_name, std::ios::binary);
        if (ifile.is_open()) {
            input = std::vector<unsigned char>(
                    std::istreambuf_iterator<char>(ifile),
                    std::istreambuf_iterator<char>());
            ifile.close();
        }

        check_result(input.size(), (input == output));
    }

    // assign std::vector from std::istreambuf_iterator iterators
    {
        input.clear();
        std::ifstream ifile(file_name, std::ios::binary);
        if (ifile.is_open()) {
            ifile.seekg(0, std::ios_base::end);
            auto length = ifile.tellg();
            ifile.seekg(0, std::ios_base::beg);

            input.reserve(static_cast<size_t>(length));
            input.assign(std::istreambuf_iterator<char>(ifile),
                    std::istreambuf_iterator<char>());
            ifile.close();
        }

        check_result(input.size(), (input == output));
    }

    // copy file content to std::vector using std::istreambuf_iterator
    // iterators and std::back_inserter adapter
    {
        input.clear();
        std::ifstream ifile(file_name, std::ios::binary);
        if (ifile.is_open()) {
            ifile.seekg(0, std::ios_base::end);
            auto length = ifile.tellg();
            ifile.seekg(0, std::ios_base::beg);

            input.reserve(static_cast<size_t>(length));
            std::copy(std::istreambuf_iterator<char>(ifile),
                    std::istreambuf_iterator<char>(),
                    std::back_inserter(input));
            ifile.close();
        }

        check_result(input.size(), (input == output));
    }

    std::filesystem::remove(file_name);
}

}
