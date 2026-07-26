#pragma once

#include "recipe_10_common.h"

#define USE_NORMAL_MUTEXLOCK
#define USE_DOUBLECHECK_LOCK

#define USE_CPP11_DOUBLECHECK_LOCK
#define USE_CPP11_CALL_ONCE_OBJECT
#define USE_CPP11_STATIC_LOCAL_OBJ

namespace recipe_10_07
{

class Singleton
{
private:
    Singleton() = default;

public:
    Singleton(Singleton const &) = delete;
    Singleton &operator =(Singleton const &) = delete;

public:
#if defined(USE_NORMAL_MUTEXLOCK)
    static std::mutex mutex1_;
    static std::unique_ptr<Singleton> instance1_;

    static Singleton &instance1() {
        std::lock_guard<std::mutex> lock(mutex1_);
        if (instance1_ == nullptr) {
            instance1_.reset(new Singleton());
        }
        return *instance1_;
    }

    void func1() {
        std::cout << "use normal mutex lock" << std::endl;
    }
#endif

#if defined(USE_DOUBLECHECK_LOCK)
    static std::mutex mutex2_;
    static std::unique_ptr<Singleton> instance2_;

    static Singleton &instance2() {
        if (instance2_ == nullptr) {
            std::lock_guard<std::mutex> lock(mutex2_);
            if (instance2_ == nullptr) {
                instance2_.reset(new Singleton());
            }
        }
        return *instance2_;
    }

    void func2() {
        std::cout << "use double check lock" << std::endl;
    }
#endif

#if defined(USE_CPP11_DOUBLECHECK_LOCK)
    static std::mutex mutex3_;
    static std::atomic<std::shared_ptr<Singleton>> instance3_;

    static Singleton &instance3() {
        std::shared_ptr<Singleton> tmp =
                instance3_.load(std::memory_order_acquire);
        if (!tmp) {
            std::lock_guard<std::mutex> lock(mutex3_);
            tmp = instance3_.load(std::memory_order_relaxed);
            if (!tmp) {
                tmp.reset(new Singleton);
                instance3_.store(tmp, std::memory_order_release);
            }
        }
        return *tmp;
    }

    void func3() {
        std::cout << "use double check lock in C++11" << std::endl;
    }
#endif

#if defined(USE_CPP11_CALL_ONCE_OBJECT)
    static std::once_flag flag_;
    static std::unique_ptr<Singleton> instance4_;

    static Singleton &instance4() {
        std::call_once(flag_, [] {
            instance4_.reset(new Singleton());
        });
        return *instance4_;
    }

    void func4() {
        std::cout << "use call once function in C++11" << std::endl;
    }
#endif

#if defined(USE_CPP11_STATIC_LOCAL_OBJ)
    static Singleton &instance() {
        static Singleton instance;
        return instance;
    }

    void func() {
        std::cout << "use static local object in C++11" << std::endl;
    }
#endif
};

#if defined(USE_NORMAL_MUTEXLOCK)
std::mutex Singleton::mutex1_;
std::unique_ptr<Singleton>Singleton::instance1_ { nullptr };
#endif

#if defined(USE_DOUBLECHECK_LOCK)
std::mutex Singleton::mutex2_;
std::unique_ptr<Singleton>Singleton::instance2_ { nullptr };
#endif

#if defined(USE_CPP11_DOUBLECHECK_LOCK)
std::mutex Singleton::mutex3_;
std::atomic<std::shared_ptr<Singleton>> Singleton::instance3_ { nullptr };
#endif

#if defined(USE_CPP11_CALL_ONCE_OBJECT)
std::once_flag Singleton::flag_;
std::unique_ptr<Singleton>Singleton::instance4_ { nullptr };
#endif

template <typename T>
class SingletonBase
{
protected:
    SingletonBase() = default;

public:
    SingletonBase(SingletonBase const &) = delete;
    SingletonBase &operator =(SingletonBase const&) = delete;

    static T &instance() {
        static T instance_;
        return instance_;
    }
};

class Single : public SingletonBase<Single>
{
private:
    Single() {
    }

public:
    void func() {
        std::cout << "use as base class template" << std::endl;
    }

    friend class SingletonBase<Single>;
};

void execute()
{
    // singleton
    {
#if defined(USE_NORMAL_MUTEXLOCK)
        auto &s1 = Singleton::instance1();
        s1.func1();
#endif

#if defined(USE_DOUBLECHECK_LOCK)
        auto &s2 = Singleton::instance2();
        s2.func2();
#endif
        std::cout << std::endl;

#if defined(USE_CPP11_DOUBLECHECK_LOCK)
        auto &s3 = Singleton::instance3();
        s3.func3();
#endif

#if defined(USE_CPP11_CALL_ONCE_OBJECT)
        auto &s4 = Singleton::instance4();
        s4.func4();
#endif

#if defined(USE_CPP11_STATIC_LOCAL_OBJ)
        auto &s = Singleton::instance();
        s.func();
#endif
        std::cout << std::endl;
    }

    // implement singleton as class template
    {
        auto &s = Single::instance();
        s.func();
    }
}

}
