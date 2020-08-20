//
// Created by Alexey Pashinov on 20/08/20
//

#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>

/// Thread Safe Queue
template <typename T>
class safe_queue
{
public:
    ///
    /// \brief Push a new instance of T at the back of the deque
    /// \param value
    void push(T&& value)
    {
        data_protected([&] { collection_.emplace(value); });
    }

    ///
    /// \brief Push a new instance of T at the back of the deque
    /// \param value
    void push(const T& value)
    {
        data_protected([&] { collection_.emplace(value); });
    }

    ///
    /// \brief Returns the front element and removes it from the collection
    /// \return
    T pop(void) noexcept
    {
        std::unique_lock<std::mutex> lock{ mutex_ };
        while (collection_.empty())
        {
            cond_.wait(lock);
        }
        auto elem = collection_.front();
        collection_.pop();
        return elem;
    }

    ///
    /// \brief Returns the front element and removes it from the collection with timeout
    /// \param ms
    /// \return
    T pop_for(std::uint32_t ms) noexcept
    {
        std::unique_lock<std::mutex> lock{ mutex_ };
        while (collection_.empty())
        {
            if (cond_.wait_for(lock, std::chrono::milliseconds(ms)) == std::cv_status::timeout)
            {
                return {};
            }
        }
        auto elem = collection_.front();
        collection_.pop();
        return elem;
    }

    typename std::queue<T>::size_type size() const noexcept
    {
        std::lock_guard<std::mutex> lock{ mutex_ };
        return collection_.size();
    }

    /// Check if queue is empty (volatile state)
    /// \return
    bool empty() const noexcept
    {
        std::lock_guard<std::mutex> lock{ mutex_ };
        return collection_.empty();
    }

private:
    ///
    /// \brief Protects the deque
    /// \tparam C
    /// \param callback
    template <class C>
    void data_protected(C&& callback)
    {
        std::lock_guard<std::mutex> lock{ mutex_ };
        callback();
        cond_.notify_one();
    }

    std::queue<T>           collection_;
    mutable std::mutex      mutex_;
    std::condition_variable cond_;
};
