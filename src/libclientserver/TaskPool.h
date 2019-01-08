
class TaskPool {
    public:
        TaskPool(unsigned int num_threads = 1) {
            while (num_threads--) {
                threads.emplace_back([this] {
                    while(true) {
                        std::unique_lock<std::mutex> lock(mutex);
                        condvar.wait(lock, [this] {return !queue.empty();});
                        auto task = std::move(queue.front());
                        if (task.valid()) {
                            queue.pop();
                            lock.unlock();
                            // run the task - this cannot throw; any exception
                            // will be stored in the corresponding future
                            task();
                        } else {
                            // an empty task is used to signal end of stream
                            // don't pop it off the top; all threads need to see it
                            break;
                        }
                    }
                });
            }
        }

    ~TaskPool() {
        {
            std::lock_guard<std::mutex> lock(mutex);
            for(size_t i =0;i<threads.size();i++)
                queue.push({});
        }
        condvar.notify_all();
        for (auto& thread : threads) {
            thread.join();
        }
    }

    template<typename F, typename R = std::result_of_t<F&&()> >
        std::future<R> Add(F&& f) const {
            auto task = std::packaged_task<R()>(std::forward<F>(f));
            auto future = task.get_future();
            {
                std::lock_guard<std::mutex> lock(mutex);
                // conversion to packaged_task<void()> erases the return type
                // so it can be stored in the queue. the future will still
                // contain the correct type
                queue.push(std::packaged_task<void()>(std::move(task)));
            }
            condvar.notify_one();
            return future;
        }

    private:
        std::vector<std::thread> threads;
        mutable std::queue<std::packaged_task<void()> > queue;
        mutable std::mutex mutex;
        mutable std::condition_variable condvar;
};

