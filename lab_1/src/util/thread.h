#pragma once
#include <thread>

namespace kki{
    class thread
    {
    private:

        static void runner_function(thread* thread_)
        {
            thread_->run();
        }

        std::thread thread_;

    public:

        thread() = default;
        thread(thread&) = delete;
        thread(thread&&) = delete;
        void operator =(thread&) = delete;
        void operator =(thread&&) = delete;
        virtual ~thread() = default;

        void join()
        {
            thread_.join();
        }

        void detach()
        {
            thread_.detach();
        }

        [[nodiscard]] bool joinable() const
        {
            return thread_.joinable();
        }

        [[nodiscard]] std::thread::id get_id() const
        {
            return thread_.get_id();
        }

        void start()
        {
            thread_ = std::thread(runner_function, this);
        }

        virtual void run() = 0;
    };
}