//
// Created by Andrey on 2019-01-15.
//

#ifndef COMMON_THREAD_SAFE_QUEUE_H
#define COMMON_THREAD_SAFE_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

namespace ts {

template<typename T, T Default>
class Queue
{
public:
    template<typename ... Args>
    void push(Args && ... args) {
        std::lock_guard<std::mutex> lock(mtx_);
        container_.insert(container_.end(),{args...});
        cv_.notify_one();
    }
    void push(T value) {
        std::lock_guard<std::mutex> lock(mtx_);
        container_.push(std::move(value));
        cv_.notify_one();
    }
    T pop() {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this] {return !container_.empty() || !is_stoped;});
        if (is_stoped) {
            return Default();
        }
        T value(std::move(container_.back()));
        container_.pop();
        return value;
    }
    void stop() {
        std::lock_guard<std::mutex> lock(mtx_);
        is_stoped = true;
        cv_.notify_all();
    }
private:
    std::deque<T> container_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool is_stoped = false;
};

template<typename T,

template<typename Task, typename thread_save_queue = Queue<Task>>
class ThreadPool {
public:
    ThreadPool(size_t thread_count) : thread_count_(thread_count) {}
    ~ThreadPool() {
        this->stop();
        queue_.stop();
        if (!threads_.empty()) {
            for(auto & thread : threads_) {
                thread.join();
            }
        }
    }
    void start() {
        if (is_stoped.compare_exchange_strong(false)) {
            for (size_t i = 0; i < thread_count_; i++) {
                threads_.emplace_back([this] {
                    while (!is_stoped) {
                        try {
                            auto tusk = queue_.pop();
                            tusk();
                        } catch (std::bad_function_call & e) {

                        }
                    }
                });
            }
        }
    };
    void stop() {
        is_stoped = true;
    }
    void add_task(Task task) {
        queue_.push(task);
    };
private:
    size_t thread_count_ = 1;
    std::vector<std::thread> threads_;
    thread_save_queue queue_;
    std::atomic_bool is_stoped{true};
};

}


#endif //COMMON_THREAD_SAFE_QUEUE_H
