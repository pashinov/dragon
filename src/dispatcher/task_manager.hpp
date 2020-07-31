//
// Created by apashinov on 31/07/20
//

#ifndef DISPATCHER_TASK_MANAGER_HPP
#define DISPATCHER_TASK_MANAGER_HPP

// 3rd party includes
#include <boost/asio.hpp>

class task_manager
{
public:
    explicit task_manager(boost::asio::io_service& io_service);
    ~task_manager();

    void start();
    void stop();
    void add_task(std::function<void()>& executor);
    void add_task(std::function<void()>& executor, std::chrono::duration<long long> expiry_time);

private:
    class impl;
    std::unique_ptr<impl> impl_;
};

#endif // DISPATCHER_TASK_MANAGER_HPP
