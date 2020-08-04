//
// Created by Alexey Pashinov on 31/07/20
//

#pragma once

// system includes
#include <chrono>

// 3rd party includes
#include <boost/asio.hpp>

namespace phoenix_service
{
    class task_manager
    {
    public:
        task_manager(boost::asio::io_service& io_service);
        ~task_manager();

        void start();
        void stop();
        void add_task(std::function<void()>& executor);
        void add_task(std::function<void()>& executor, std::chrono::duration<long long> expiry_time);

    private:
        class impl;
        std::unique_ptr<impl> impl_;
    };
}
