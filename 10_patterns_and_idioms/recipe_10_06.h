#pragma once

#include "recipe_10_common.h"

namespace recipe_10_06
{

namespace demo1
{

template <typename T>
class base
{
public:
    void func() {
        static_cast<T *>(this)->func();
    }
};

class derived : public base<derived>
{
public:
    void func() {
        std::cout << "derived::func" << std::endl;
    }
};

template <typename T>
void do_something(base<T> &b)
{
    b.func();
}

}

namespace demo2
{

template <typename T>
class control
{
public:
    void draw() {
        static_cast<T *>(this)->erase_background();
        static_cast<T *>(this)->paint();
    }
};

class button : public control<button>
{
private:
    void erase_background() {
        std::cout << "erasing button background..." << std::endl;
    }

    void paint() {
        std::cout << "painting button..." << std::endl;
    }

    friend class control<button>;
};

class checkbox : public control<checkbox>
{
public:
    void erase_background() {
        std::cout << "erasing checkbox background..." << std::endl;
    }

    void paint() {
        std::cout << "painting checkbox..." << std::endl;
    }
};

template <typename T>
void draw_control(control<T> &c)
{
    c.draw();
}

template <typename T>
class fancybutton : public control<fancybutton<T>>
{
public:
    void erase_background() {
        static_cast<T *>(this)->paint_area();
    }

    void paint() {
        std::cout << "painting fancybutton..." << std::endl;
    }
};

class transparentbutton : public fancybutton<transparentbutton>
{
public:
    void paint_area() {
        std::cout << "painting transparent button background..." << std::endl;
    }
};

}

namespace demo3
{
    class controlbase
    {
    public:
        virtual void draw() = 0;

        virtual ~controlbase() {
        }
    };

    template <typename T>
    class control : public controlbase
    {
    private:
        T *derived() {
            return static_cast<T *>(this);
        }

    public:
        virtual void draw() override {
            derived()->erase_background();
            derived()->paint();
        }
    };

    class button : public control<button>
    {
    public:
        void erase_background() {
            std::cout << "erasing button background..." << std::endl;
        }

        void paint() {
            std::cout << "painting button..." << std::endl;
        }
    };

    class checkbox : public control<checkbox>
    {
    public:
        void erase_background() {
            std::cout << "erasing checkbox background..." << std::endl;
        }

        void paint() {
            std::cout << "painting checkbox..." << std::endl;
        }
    };

    void draw_controls(std::vector<std::unique_ptr<controlbase>> &v)
    {
        for (auto &c : v) {
            c->draw();
        }
    }
}

void execute()
{
    // curiously recurring template pattern (CRTP)
    {
        using namespace demo1;

        derived d;
        d.func();

        do_something(d);
        std::cout << std::endl;
    }

    // apply CRTP to control class hierarchy
    {
        using namespace demo2;

        button b;
        b.draw();

        checkbox c;
        c.draw();

        draw_control(b);
        draw_control(c);
        std::cout << std::endl;
    }

    // add more CRTP to control class hierarchy
    {
        using namespace demo2;

        transparentbutton t;
        t.draw();

        draw_control(t);
        std::cout << std::endl;
    }

    // store CRTP types in a container
    {
        using namespace demo3;

        std::vector<std::unique_ptr<controlbase>> v;
        v.emplace_back(std::make_unique<button>());
        v.emplace_back(std::make_unique<checkbox>());
        draw_controls(v);
    }
}

}
