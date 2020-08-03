//
// Created by Alexey Pashinov on 31/07/20
//

#ifndef UTILS_SIGNAL_HANDLER_HPP
#define UTILS_SIGNAL_HANDLER_HPP

// system includes
#include <condition_variable>
#include <csignal>
#include <mutex>

static std::condition_variable condition_;
static std::mutex mutex_;

namespace utils
{
    class signal_handler
    {
    public:
        static void hook_signal(int signal_name)
        {
            signal(signal_name, handle_interrupt);
        }

        static void handle_interrupt(int signal)
        {
            switch (signal)
            {
                case SIGINT:
                case SIGTERM:
                    condition_.notify_one();
                    break;
                default:
                    break;
            }
        }

        static void wait_for_signal_interrupt()
        {
            std::unique_lock<std::mutex> lock { mutex_ };
            condition_.wait(lock);
            lock.unlock();
        }
    };
} // namespace utils

#endif // UTILS_SIGNAL_HANDLER_HPP
