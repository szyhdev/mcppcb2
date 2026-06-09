#pragma once

#include "recipe_01_common.h"

namespace recipe_01_09
{

// #define USE_PREDICATE

template <typename T, size_t const Size>
class dummy_array
{
    T data[Size] = {};

public:
    T const &GetAt(size_t const index) const {
        if (index < Size) {
            return data[index];
        }

        throw std::out_of_range("index out of range");
    }

    void SetAt(size_t const index, T const &value) {
        if (index < Size) {
            data[index] = value;
        } else {
            throw std::out_of_range("index out of range");
        }
    }

    size_t GetSize() const {
        return Size;
    }
};

template <typename T, typename C, size_t const Size>
class dummy_array_iterator_type
{
public:
    dummy_array_iterator_type(C &collection, size_t const index) :
            index(index),
            collection(collection) {
    }

    bool operator !=(dummy_array_iterator_type const &other) const {
        return index != other.index;
    }

    T const &operator *() const {
        return collection.GetAt(index);
    }

    dummy_array_iterator_type &operator ++() {
        ++index;
        return *this;
    }

    dummy_array_iterator_type operator ++(int) {
        auto temp = *this;
        ++*this;
        return temp;
    }

private:
    size_t index;
    C &collection;
};

struct stop_at_sentinel
{
    int value;

    template <typename Iterator>
    friend bool operator !=(Iterator itr, stop_at_sentinel sas) {
        return *itr < sas.value;
    }
};

template <typename T, size_t const Size>
using dummy_array_iterator =
        dummy_array_iterator_type<T, dummy_array<T, Size>, Size>;

template <typename T, size_t const Size>
using dummy_array_const_iterator =
        dummy_array_iterator_type<T, dummy_array<T, Size> const, Size>;

template <typename T, size_t const Size>
inline dummy_array_iterator<T, Size> begin(dummy_array<T, Size> &collection)
{
    return dummy_array_iterator<T, Size>(collection, 0);
}

#if !defined(USE_PREDICATE)

template <typename T, size_t const Size>
inline dummy_array_iterator<T, Size> end(dummy_array<T, Size> &collection)
{
    return dummy_array_iterator<T, Size>(collection, collection.GetSize());
}

#else

template <typename T, size_t const Size>
inline stop_at_sentinel end(dummy_array<T, Size> &collection)
{
    return stop_at_sentinel { 8 };
}

#endif

template <typename T, size_t const Size>
inline dummy_array_const_iterator<T, Size> begin(dummy_array<T, Size> const &collection)
{
    return dummy_array_const_iterator<T, Size>(collection, 0);
}

#if !defined(USE_PREDICATE)

template <typename T, size_t const Size>
inline dummy_array_const_iterator<T, Size> end(dummy_array<T, Size> const &collection)
{
    return dummy_array_const_iterator<T, Size>(collection, collection.GetSize());
}

#else

template <typename T, size_t const Size>
inline stop_at_sentinel end(dummy_array<T, Size> const &collection)
{
    return stop_at_sentinel { 8 };
}

#endif

template <typename T, const size_t Size>
void print_dummy_array(dummy_array<T, Size> const &da)
{
    for (auto &&e : da) {
        std::cout << e << " ";
    }
    std::cout << std::endl;
}

void execute()
{
    // enable range-based for loop for custom type
    {
        dummy_array<int, 8> da;
        da.SetAt(0, 0);
        da.SetAt(1, 1);
        da.SetAt(2, 1);
        da.SetAt(3, 2);
        da.SetAt(4, 3);
        da.SetAt(5, 5);
        da.SetAt(6, 8);
        da.SetAt(7, 13);

        print_dummy_array(da);
    }
}

}
