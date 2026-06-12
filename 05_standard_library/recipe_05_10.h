#pragma once

#include "recipe_05_common.h"

namespace recipe_05_10
{

template <typename Type, size_t const SIZE>
class dummy_array
{
    Type data[SIZE] = {};

public:
    Type &operator [](size_t const index) {
        if (index < SIZE) {
            return data[index];
        }
        throw std::out_of_range("index out of range");
    }

    Type const &operator [](size_t const index) const {
        if (index < SIZE) {
            return data[index];
        }
        throw std::out_of_range("index out of range");
    }

    size_t size() const {
        return SIZE;
    }

    template <typename T, size_t const Size>
    class dummy_array_iterator
    {
    public:
        // typedefs that define standard synonyms
        typedef dummy_array_iterator self_type;
        typedef std::random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef T &reference;
        typedef T *pointer;
        typedef ptrdiff_t difference_type;

    private:
        // private members and method
        pointer ptr = nullptr;
        size_t index = 0;

        bool compatible(self_type const &other) const {
            return ptr == other.ptr;
        }

    public:
        // explicit constructor specific to this class
        explicit dummy_array_iterator(pointer ptr, size_t const index) :
                ptr(ptr), index(index) {
        }

        // common to all iterators: begin
        dummy_array_iterator(dummy_array_iterator const &other) = default;

        dummy_array_iterator &operator =(dummy_array_iterator const &other) = default;

        ~dummy_array_iterator() = default;

        self_type &operator ++() {
            if (index >= Size) {
                throw std::out_of_range("index out of range");
            }
            ++index;
            return *this;
        }

        self_type operator ++(int) {
            self_type tmp = *this;
            ++*this;
            return tmp;
        }
        // common to all iterators: end

        // specific to input iterator: begin
        bool operator ==(self_type const &other) const {
            assert(compatible(other));
            return index == other.index;
        }

        bool operator !=(self_type const &other) const {
            return !(*this == other);
        }

        reference operator *() const {
            if (ptr == nullptr) {
                throw std::bad_function_call();
            }
            return *(ptr + index);
        }

        reference operator ->() const {
            if (ptr == nullptr) {
                throw std::bad_function_call();
            }
            return *(ptr + index);
        }
        // specific to input iterator: end

        // specific to output iterator: begin
        // can be dereferenced as an lvalue
        // specific to output iterator: end

        // specific to forward iterator: begin
        // default-constructible
        dummy_array_iterator() = default;

        // multi-pass: neither dereferencing nor incrementing affects dereferenceability
        // specific to forward iterator: end

        // specific to bidirectional iterator: begin
        // can be decremented
        self_type &operator --() {
            if (index <= 0) {
                throw std::out_of_range("index out of range");
            }
            --index;
            return *this;
        }

        self_type operator --(int) {
            self_type tmp = *this;
            --*this;
            return tmp;
        }
        // specific to bidirectional iterator: end

        // specific to random access iterator: begin
        self_type &operator +=(difference_type const offset) {
            if (((index + offset) < 0) || ((index + offset) > Size)) {
                throw std::out_of_range("index out of range");
            }
            index += offset;
            return *this;
        }

        self_type &operator -=(difference_type const offset) {
            return *this += -offset;
        }

        self_type operator +(difference_type offset) const {
            self_type tmp = *this;
            return tmp += offset;
        }

        self_type operator -(difference_type offset) const {
            self_type tmp = *this;
            return tmp -= offset;
        }

        difference_type operator -(self_type const &other) const {
            assert(compatible(other));
            return (index - other.index);
        }

        bool operator <(self_type const &other) const {
            assert(compatible(other));
            return index < other.index;
        }

        bool operator >=(self_type const &other) const {
            return !(*this < other);
        }

        bool operator >(self_type const &other) const {
            return other < *this;
        }

        bool operator <=(self_type const &other) const {
            return !(other < *this);
        }

        value_type &operator [](difference_type const offset) {
            return (*(*this + offset));
        }

        value_type const &operator [](difference_type const offset) const {
            return (*(*this + offset));
        }
        // specific to random access iterator: end
    };

    typedef dummy_array_iterator<Type, SIZE> iterator;
    typedef dummy_array_iterator<Type const, SIZE> constant_iterator;

public:
    iterator begin() {
        return iterator(data, 0);
    }

    iterator end() {
        return iterator(data, SIZE);
    }

    constant_iterator begin() const {
        return constant_iterator(data, 0);
    }

    constant_iterator end() const {
        return constant_iterator(data, SIZE);
    }
};

void execute()
{
    // operations of dummy_array_iterator
    {
        dummy_array<int, 5> a;
        a[0] = 1;
        a[1] = 2;
        a[2] = 3;
        a[3] = 4;
        a[4] = 5;

        // converting-constructible and destructible
        // copy-constructible, copy-assignable
        // incrementable and decrementable
        // arithmetic increment and decrement
        // compound assignment
        dummy_array<int, 5>::iterator ai1 = a.begin();
        auto ai2(ai1);
        ai2++;
        dummy_array<int, 5>::iterator ai3;
        ai3 = ai2;
        ++ai3;
        auto ai4 = a.end();
        ai4--;
        --ai4;
        auto ai5 = ai1;
        ai5 = ai1 + 3;
        ai5 = ai3 - 2;
        ai5 += 3;
        ai5 -= 2;
        ai5 -= -4;
        ai5 += -1;
        auto aie = a.end();
        std::cout << *ai1 << ", " << *ai2 << ", " << *ai3 << ", " << *ai4 <<
                ", " << *ai5 << std::endl;

        // equality/inequality comparison
        assert(ai1 != ai2);
        assert(aie == a.end());

        // logical comparison
        auto ls = ai1 < ai4;
        auto gt = ai1 > ai4;
        auto le = ai1 <= ai4;
        auto ge = ai1 >= ai4;
        std::cout << std::boolalpha << ls << ", " << gt << ", " <<
                le << ", " << ge << std::endl;

        // dereferenced as rvalue and lvalue
        std::cout << (*ai1 > 0) << std::endl;
        *ai1 = 11;
        std::cout << *ai1 << std::endl;

        // offset dereference operator
        std::cout << ++ai1[4] << ", " << *ai5 << std::endl;

        // multi-pass
        ai2 = ai1;
        ai2++;
        std::cout << *ai1 << ", " << *ai2 << std::endl;

        // compatible with other iterator of the same type
        dummy_array<int, 5> b;
        b[0] = 1;
        b[1] = 2;
        b[2] = 3;
        b[3] = 4;
        b[4] = 5;
        dummy_array<int, 5>::iterator bi1 = b.begin();
        // assert(ai1 != bi1);  // assert failure

        std::cout << std::endl;
    }

    // operations of dummy_array
    {
        // work with algorithm
        dummy_array<int, 5> a;
        a[0] = 1;
        a[1] = 2;
        a[2] = 3;
        a[3] = 4;
        a[4] = 5;

        std::transform(a.begin(), a.end(), a.begin(), [] (int const e) {
            return e * 2;
        });

        for (auto &&e : a) {
            std::cout << e << " ";
        }
        std::cout << std::endl;

        // work with lambda
        auto lp1 = [] (dummy_array<int, 5> const &a) {
            for (auto const &e : a) {
                std::cout << e << " ";
            }
            std::cout << std::endl;
        };

        lp1(a);

        // work with user-defined type
        dummy_array<recipe_05_common::task, 5> ta1;
        ta1[0] = recipe_05_common::task { 1, "task1" };
        ta1[1] = recipe_05_common::task { 2, "task2" };
        ta1[2] = recipe_05_common::task { 3, "task3" };
        ta1[3] = recipe_05_common::task { 4, "task4" };
        ta1[4] = recipe_05_common::task { 5, "task5" };

        for (auto &&e : ta1) {
            std::cout << "(" << e.priority << ", " << e.name << ") ";
        }
        std::cout << std::endl;

        auto pa1 = &ta1[2];
        std::cout << "(" << pa1->priority << ", " << pa1->name << ")" << std::endl;

        // work with smart pointer
        dummy_array<std::unique_ptr<recipe_05_common::task>, 5> ta2;
        ta2[0] = std::make_unique<recipe_05_common::task>(1, "task1");
        ta2[1] = std::make_unique<recipe_05_common::task>(2, "task2");
        ta2[2] = std::make_unique<recipe_05_common::task>(3, "task3");
        ta2[3] = std::make_unique<recipe_05_common::task>(4, "task4");
        ta2[4] = std::make_unique<recipe_05_common::task>(5, "task5");

        for (auto itr = ta2.begin(); itr != ta2.end(); ++itr) {
            std::cout << "(" << itr->priority << ", " << itr->name << ") ";
        }
        std::cout << std::endl;

        auto pa2 = ta2[2].get();
        std::cout << "(" << pa2->priority << ", " << pa2->name << ")" << std::endl;

        // work as an array
        auto lp2 = [] (dummy_array<int, 5> const &a) {
            for (size_t i = 0; i < a.size(); ++i) {
                std::cout << a[i] << " ";
            }
            std::cout << std::endl;
        };

        lp2(a);
    }
}

}
