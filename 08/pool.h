#ifndef POOL_H
#define POOL_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <vector>
#include <queue>
#include <memory>
#include <stdexcept>

class ThreadPool
{

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool end;

public:
    explicit ThreadPool(size_t poolSize) : end(false)
    {
        for (size_t i = 0; i < poolSize; ++i) {
            workers.emplace_back(
                [this]
                {
                    while (true)
                    {
                        std::function<void()> task;

                        {
                            std::unique_lock<std::mutex> lock(this->queue_mutex);
                            
                            this->condition.wait(lock,
                                [this] 
                                {
                                    return this->end || !this->tasks.empty(); 
                                });
                            
                            if (this->end && this->tasks.empty()) {
                                return;
                            }
                            
                            task = std::move(this->tasks.front());
                            this->tasks.pop();
                        }

                        task();
                    }
                }
            );
        }
    }

    template <class Func, class ...Args>
    auto exec(Func &&func, Args &&...args) -> std::future<decltype(func(args...))>
    {
        auto task = std::make_shared<std::packaged_task<decltype(func(args...))()>>
            (std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
        
        auto result = task->get_future();
        
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            
            if (end) {
                throw std::runtime_error("New task for closed pool");
            }
            
            tasks.emplace([task]() { (*task)(); });
        }
        
        condition.notify_one();
        return result;
    }

    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            end = true;
        }

        condition.notify_all();
        for (auto &worker : workers) {
            worker.join();
        }
    }

};



#endif