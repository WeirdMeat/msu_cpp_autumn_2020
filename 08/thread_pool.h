#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>

class ThreadPool
{
private:
    std::vector<std::thread> threads;
    std::queue<std::function <void()>> thread_pool;
    std::atomic<bool> done;
    std::mutex locker;
    std::condition_variable new_task;
public:
    explicit ThreadPool(size_t poolSize): done(false) {
        for (size_t i = 0; i < poolSize; i++) {
            threads.emplace_back([this]() {
                while (!done) {
                    std::unique_lock<std::mutex> lock(locker);
                    if (thread_pool.empty()) {
                        next_task().wait(lock);
                    } else {
                        auto next_task = std::move(thread_pool.front());
                        thread_pool.pop();
                        locker.unlock();
                        next_task();
                    }
                }
            });
        }
    }
    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))>
    {
        auto task = std::make_shared<std::packaged_task<decltype(func(args...))()>>([func, args...]() { return func(args...); });
        std::unique_lock<std::mutex> lock(locker);
        thread_pool.push([task]() { (*task)(); });
        new_task.notify_one();
        return task->get_future();
    }
    ~ThreadPool() {
        done = false;
        new_task.notify_all();
        for (auto& i : threads) {
            i.join();
        }
    }
};

void test();
