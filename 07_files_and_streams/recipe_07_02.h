#pragma once

#include <iostream>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <vector>

namespace recipe_07_02
{

using namespace std::string_literals;

class foo
{
    int i;
    char c;
    std::string s;

public:
    foo(int const i = 0, char const c = 0, std::string const &s = {})
            : i(i), c(c), s(s) {
    }

    foo(foo const &) = default;
    foo &operator =(foo const &) = default;

    bool operator ==(foo const &rhv) const {
        return (i == rhv.i) && (c == rhv.c) && (s == rhv.s);
    }

    bool operator !=(foo const &rhv) const {
        return !(*this == rhv);
    }

    size_t size() {
        return sizeof(i) + sizeof(c) + sizeof(int) + s.size();
    }

    bool write(std::ofstream &ofile, size_t &writtenbyte) const {
        ofile.write(reinterpret_cast<const char *>(&i), sizeof(i));
        writtenbyte += sizeof(i);

        ofile.write(&c, sizeof(c));
        writtenbyte += sizeof(c);

        int size = static_cast<int>(s.size());
        ofile.write(reinterpret_cast<char *>(&size), sizeof(size));
        writtenbyte += sizeof(size);

        ofile.write(s.data(), s.size());
        writtenbyte += size;

        return !ofile.fail();
    }

    bool read(std::ifstream &ifile, size_t &readbyte)
    {
        ifile.read(reinterpret_cast<char *>(&i), sizeof(i));
        readbyte += sizeof(i);

        ifile.read(&c, sizeof(c));
        readbyte += sizeof(c);

        int size { 0 };
        ifile.read(reinterpret_cast<char *>(&size), sizeof(size));
        readbyte += sizeof(size);

        s.resize(size);
        ifile.read(reinterpret_cast<char *>(&s.front()), size);
        readbyte += size;

        return !ifile.fail();
    }

    friend std::ofstream &operator <<(std::ofstream &ofile, foo const &f);
    friend std::ifstream &operator >>(std::ifstream &ifile, foo &f);
};

std::ofstream &operator <<(std::ofstream &ofile, foo const &f)
{
    ofile.write(reinterpret_cast<const char *>(&f.i), sizeof(f.i));
    ofile.write(&f.c, sizeof(f.c));
    int size = static_cast<int>(f.s.size());
    ofile.write(reinterpret_cast<char *>(&size), sizeof(size));
    ofile.write(f.s.data(), f.s.size());

    return ofile;
}

std::ifstream &operator >>(std::ifstream &ifile, foo &f)
{
    ifile.read(reinterpret_cast<char *>(&f.i), sizeof(f.i));
    ifile.read(&f.c, sizeof(f.c));
    int size { 0 };
    ifile.read(reinterpret_cast<char *>(&size), sizeof(size));
    f.s.resize(size);
    ifile.read(reinterpret_cast<char *>(&f.s.front()), size);

    return ifile;
}

struct foopod
{
    bool a;
    char b;
    int c[2];
};

bool operator ==(foopod const &fp1, foopod const &fp2)
{
    return (fp1.a == fp2.a) && (fp1.b == fp2.b) &&
            (fp1.c[0] == fp2.c[0]) && (fp1.c[1] == fp2.c[1]);
}

void execute()
{
    const char *file_name = "sample.bin";

    auto check_result = [&file_name] (size_t length, bool is_equal) {
        std::cout << length << " read: " << is_equal << std::endl;
    };

    std::cout << std::boolalpha;

    // serialize/deserialize POD types that do not contain pointers
    {
        std::vector<foopod> output {
            { true, '1', { 1, 2 } },
            { true, '2', { 2, 4 } },
            { false, '3', { 3, 6 } }
        };
        std::vector<foopod> input;

        // serialize objects to a binary file
        size_t length = 0;
        std::ofstream ofile(file_name, std::ios::binary);
        if (ofile.is_open()) {
            for (auto const &value : output) {
                ofile.write(reinterpret_cast<const char *>(&value), sizeof(value));
                length += sizeof(value);
                assert(!ofile.fail());
            }
            ofile.close();
        }

        std::cout << length << " written" << std::endl;

        // deserialize objects from a binary file
        length = 0;
        std::ifstream ifile(file_name, std::ios::binary);
        if (ifile.is_open()) {
            while (true) {
                foopod value;
                ifile.read(reinterpret_cast<char *>(&value), sizeof(value));
                if (ifile.fail() || ifile.eof()) {
                    break;
                }
                length += sizeof(value);
                input.push_back(value);
            }
            ifile.close();
        }

        check_result(length, (input == output));
    }

    // serialize/deserialize non-POD types (or POD types that contain pointers): write and read
    {
        auto f1 = foo { 1, '1', "one"s };

        // serialize non-POD types
        size_t writtenbyte = 0;
        std::ofstream ofile(file_name, std::ios::binary);
        if (ofile.is_open()) {
            f1.write(ofile, writtenbyte);
            ofile.close();
        }

        std::cout << writtenbyte << " written" << std::endl;

        // deserialize non-POD types
        auto f2 = foo {};
        size_t readbyte = 0;
        std::ifstream ifile(file_name, std::ios::binary);
        if (ifile.is_open()) {
            f2.read(ifile, readbyte);
            ifile.close();
        }

        check_result(readbyte, (f2 == f1));
    }

    // serialize/deserialize non-POD types (or POD types that contain pointers): << and >>
    {
        auto f1 = foo { 1, '1', "one"s };

        // serialize non-POD types

        std::ofstream ofile(file_name, std::ios::binary);
        if (ofile.is_open()) {
            ofile << f1;
            ofile.close();
        }

        std::cout << f1.size() << " written" << std::endl;

        // deserialize non-POD types
        auto f2 = foo {};
        std::ifstream ifile(file_name, std::ios::binary);
        if (ifile.is_open()) {
            ifile >> f2;
            ifile.close();
        }

        check_result(f2.size(), (f2 == f1));
    }
}

}
