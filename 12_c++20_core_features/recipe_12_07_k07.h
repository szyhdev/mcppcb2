#pragma once

#include "recipe_12_common.h"

namespace recipe_12_07_k07
{

#define COROUTINE_APPROACH
#define THREAD_APPROACH

#define OPTIMIZATION_ON

struct timed_task
{
    std::function<void(int)> callback;
    std::chrono::steady_clock::time_point expire_time;
    int value;

    bool operator >(const timed_task &other) const {
        return expire_time > other.expire_time;
    }
};

class timed_task_queue
{
public:
    static timed_task_queue& get_instance() {
        static timed_task_queue ttq;
        return ttq;
    }

    void add_task(std::function<void(int)> callback,
            std::chrono::milliseconds duration, int id) {
        timed_task task;
        task.callback = std::move(callback);
        task.expire_time = std::chrono::steady_clock::now() + duration;
        task.value = id * 10;

        {
            std::lock_guard<std::mutex> lock(mutex_);
            tasks_.push(std::move(task));
        }

        cond_var_.notify_one();
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            stop_flag_ = true;
        }

        cond_var_.notify_all();

        if (worker_thread_.joinable()) {
            worker_thread_.join();
        }
    }

private:
    timed_task_queue(const timed_task_queue &) = delete;
    timed_task_queue &operator =(const timed_task_queue &) = delete;

    timed_task_queue() : stop_flag_(false) {
        worker_thread_ = std::thread(&timed_task_queue::process_loop, this);
    }

    ~timed_task_queue() {
        stop();
    }

private:
    void process_loop() {
        while (true) {
            std::unique_lock<std::mutex> lock(mutex_);

            cond_var_.wait(lock, [this] {
                return stop_flag_ || !tasks_.empty();
            });

            if (stop_flag_ && tasks_.empty()) {
                break;
            }

            if (tasks_.empty()) {
                continue;
            }

            auto& top_task = tasks_.top();
            auto now = std::chrono::steady_clock::now();

            if (now >= top_task.expire_time) {
                auto callback = std::move(top_task.callback);
                auto value = top_task.value;
                tasks_.pop();
                lock.unlock();

                try {
                    int sum = 0;
                    while (std::chrono::steady_clock::now() < top_task.expire_time) {
                        sum += 1;
                    }

                    callback(value);
                } catch (const std::exception &e) {
                    std::cout << "timed_task_queue::process_loop: " << e.what() << std::endl;
                }
            } else {
                auto wait_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                    top_task.expire_time - now
                );

                cond_var_.wait_for(lock, wait_duration);
            }
        }
    }

private:
    std::priority_queue<timed_task, std::vector<timed_task>,
            std::greater<timed_task>> tasks_;
    std::thread worker_thread_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
    bool stop_flag_;
};

#ifdef OPTIMIZATION_ON
struct final_awaiter
{
    bool await_ready() noexcept {
        return false;
    }

    template <typename T>
    std::coroutine_handle<> await_suspend(std::coroutine_handle<T> handle) noexcept {
        auto &p = handle.promise();
        if (p.continuation_) {
            return p.continuation_;
        }
        return std::noop_coroutine();
    }

    void await_resume() noexcept {
    }
};
#endif

struct promise_base
{
    auto initial_suspend() noexcept {
        recipe_12_common::print_debug_info("promise_base::initial_suspend");
        return std::suspend_always {};
    }

#ifdef OPTIMIZATION_ON
    final_awaiter final_suspend() noexcept {
        recipe_12_common::print_debug_info("promise_base::final_suspend");
        return {};
    }
#else
    auto final_suspend() noexcept {
        recipe_12_common::print_debug_info("promise_base::final_suspend");
        return std::suspend_always {};
    }
#endif

    void unhandled_exception() {
        recipe_12_common::print_debug_info("promise_base::unhandled_exception");
        std::terminate();
    }

#ifdef OPTIMIZATION_ON
    void set_continuation(std::coroutine_handle<> continuation) {
        recipe_12_common::print_debug_info("promise_base::set_continuation");
        continuation_ = continuation;
    }

private:
    std::coroutine_handle<> continuation_ = nullptr;

    friend struct final_awaiter;
#endif
};

template <typename T>
struct promise : public promise_base
{
    // auto get_return_object() {
    //     auto handle = std::coroutine_handle<promise<T>>::from_promise(*this);
    //     recipe_12_common::print_debug_info("promise<T>::get_return_object: ", &handle.promise());
    //     return handle;
    // }

    template <typename V, typename = std::enable_if_t<std::is_convertible_v<V &&, T>>>
    void return_value(V &&value) noexcept(std::is_nothrow_constructible_v<T, V &&>) {
        recipe_12_common::print_debug_info("promise<T>::return_value");
        value_ = value;
    }

    T get_value() const noexcept {
        recipe_12_common::print_debug_info("promise<T>::get_value");
        return value_;
    }

private:
    T value_;
};

template <>
struct promise<void> : public promise_base
{
    // auto get_return_object() {
    //     auto handle = std::coroutine_handle<promise<void>>::from_promise(*this);
    //     recipe_12_common::print_debug_info("promise<void>::get_return_object: ", &handle.promise());
    //     return handle;
    // }

    void return_void() noexcept {
        recipe_12_common::print_debug_info("promise<void>::return_void");
    }
};

template <typename T>
struct promise<T &> : public promise_base
{
    // auto get_return_object() {
    //     auto handle = std::coroutine_handle<promise<T &>>::from_promise(*this);
    //     recipe_12_common::print_debug_info("promise<T &>::get_return_object: ", &handle.promise());
    //     return handle;
    // }

    void return_value(T &value) noexcept {
        recipe_12_common::print_debug_info("promise<T &>::return_value");
        value_ = std::addressof(value);
    }

    T &get_value() const noexcept {
        recipe_12_common::print_debug_info("promise<T &>::get_value");
        return *value_;
    }

private:
    T *value_ = nullptr;
};

struct code_request_promise : promise<void>
{
    code_request_promise() = default;

    code_request_promise(
            std::vector<int> &codes, std::chrono::milliseconds duration, int id)
            : codes_(std::addressof(codes)), duration_(duration), id_(id) {
        recipe_12_common::print_debug_info("code_request_promise::code_request_promise");
    }

    auto get_return_object() {
        auto handle = std::coroutine_handle<code_request_promise>::from_promise(*this);
        recipe_12_common::print_debug_info("code_request_promise::get_return_object: ", &handle.promise());
        return handle;
    }

    std::vector<int> &get_codes() const noexcept {
        recipe_12_common::print_debug_info("code_request_promise::get_codes");
        return *codes_;
    }

    std::chrono::milliseconds get_duration() const noexcept {
        recipe_12_common::print_debug_info("code_request_promise::get_duration");
        return duration_;
    }

    int get_id() const noexcept {
        recipe_12_common::print_debug_info("code_request_promise::get_id");
        return id_;
    }

private:
    std::vector<int> *codes_;
    std::chrono::milliseconds duration_;
    int id_;
};

struct code_request_awaiter
{
    using promise_type = code_request_promise;

    code_request_awaiter(std::coroutine_handle<promise_type> handle) noexcept
            : handle_(handle) {
        recipe_12_common::print_debug_info("code_request_awaiter::code_request_awaiter");
    }

    bool await_ready() const noexcept {
        recipe_12_common::print_debug_info("code_request_awaiter::await_ready");
        return !handle_ || handle_.done();
    }

    void await_suspend([[maybe_unused]] std::coroutine_handle<> continuation) noexcept {
        recipe_12_common::print_debug_info("code_request_awaiter::await_suspend");
#ifdef OPTIMIZATION_ON
        handle_.promise().set_continuation(continuation);
#endif
        timed_task_queue::get_instance().add_task(
                std::bind(&code_request_awaiter::on_response, this, std::placeholders::_1),
                handle_.promise().get_duration(), handle_.promise().get_id());
        handle_.resume();
    }

    void await_resume() {
        recipe_12_common::print_debug_info("code_request_awaiter::await_resume");
    }

    void on_response(int value) {
        recipe_12_common::print_debug_info("code_request_awaiter::on_response");
        handle_.promise().get_codes().push_back(value);
    }

private:
    std::coroutine_handle<promise_type> handle_;
};

template <typename P>
struct task
{
    using promise_type = P;

    task(std::coroutine_handle<promise_type> handle) : handle_(handle) {
        recipe_12_common::print_debug_info("task::task: ", &handle_.promise());
    }

    ~task() {
        recipe_12_common::print_debug_info("task::~task: ", &handle_.promise());
        if (handle_) {
            handle_.destroy();
        }
    }

    task(task &&t) noexcept : handle_(t.handle_) {
        recipe_12_common::print_debug_info("task::task(task &&)");
        t.handle_ = nullptr;
    }

    task &operator =(task &&other) noexcept {
        recipe_12_common::print_debug_info("task::operator =");
        if (std::addressof(other) != this) {
            if (handle_) {
                handle_.destroy();
            }
            handle_ = other.handle_;
            other.handle_ = nullptr;
        }
        return *this;
    }

    task(task const &) = delete;
    task &operator =(task const &) = delete;

    bool is_done() noexcept {
        recipe_12_common::print_debug_info("task::is_done: ", &handle_.promise());
        return !handle_ || handle_.done();
    }

    void resume() noexcept {
        recipe_12_common::print_debug_info("task::resume: ", &handle_.promise());
        if (handle_) {
            handle_.resume();
        }
    }

protected:
    std::coroutine_handle<promise_type> handle_ = nullptr;
};

struct code_request_task : public task<code_request_promise>
{
    using promise_type = code_request_promise;

    code_request_task(std::coroutine_handle<promise_type> handle)
            : task<code_request_promise>(handle) {
        recipe_12_common::print_debug_info("code_request_task::code_request_task");
    }

    // T value() const noexcept {
    //     recipe_12_common::print_debug_info("code_request_task::value");
    //     return handle_.promise().get_value();
    // }

    auto operator co_await() const & noexcept {
        recipe_12_common::print_debug_info("code_request_task::operator co_await: ",
                handle_.promise().get_id());
        return code_request_awaiter { handle_ };
    }
};

code_request_task get_url(std::vector<int> &codes,
        std::chrono::milliseconds duration, int i)
{
    co_return;
}

void coroutine_approach(std::vector<int> &codes,
        std::chrono::milliseconds duration, int count)
{
    auto get_all_urls = [&codes, duration, count] () -> code_request_task {
        for (int i = 0; i < count; ++i) {
            co_await get_url(codes, duration, i);
        }
    };

    recipe_12_common::execute(get_all_urls());
}

void thread_approach(std::vector<int> &codes,
        std::chrono::milliseconds duration, int count)
{
    std::mutex mtx;
    std::vector<std::thread> threads;
    threads.reserve(count);

    for (int i = 0; i < count; ++i) {
        threads.emplace_back([&codes, &duration, &mtx] (int id) {
            auto start = std::chrono::steady_clock::now();
            int sum = 0;
            while (std::chrono::steady_clock::now() - start < duration) {
                sum += 1;
            }

            {
                std::lock_guard<std::mutex> lock(mtx);
                codes.push_back(id * 10);
            }
        }, i);
    };

    for (auto &t : threads) {
        t.join();
    }
}

void execute()
{
    constexpr std::chrono::milliseconds duration(300);
    constexpr int count = 5000;

#ifdef COROUTINE_APPROACH
    std::vector<int> codes_coroutine;
    codes_coroutine.reserve(count);
    auto tc = recipe_common::perf_timer<std::chrono::milliseconds>::duration(
            [&codes_coroutine, duration, count] () {
        coroutine_approach(codes_coroutine, duration, count);
        timed_task_queue::get_instance().stop();
    });
    std::cout << tc << std::endl;
#endif

#ifdef THREAD_APPROACH
    std::vector<int> codes_thread;
    codes_thread.reserve(count);
    auto tt = recipe_common::perf_timer<std::chrono::milliseconds>::duration(
            [&codes_thread, duration, count] () {
        thread_approach(codes_thread, duration, count);
    });
    std::cout << tt << std::endl;
#endif

#if defined(COROUTINE_APPROACH) && defined(THREAD_APPROACH)
    // recipe_common::print_collection(codes_coroutine, "codes_coroutine: ");
    std::sort(codes_coroutine.begin(), codes_coroutine.end());
    // recipe_common::print_collection(codes_thread, "codes_thread: ");
    std::sort(codes_thread.begin(), codes_thread.end());
    assert(codes_coroutine == codes_thread);
#endif
}

}
