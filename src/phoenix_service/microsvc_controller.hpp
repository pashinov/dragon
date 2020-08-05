//
// Created by Alexey Pashinov on 3/08/20
//

#pragma once

// system includes
#include <thread>

// 3rdparty includes
#include <boost/asio.hpp>

// project includes
#include <phoenix_service/task_manager.hpp>

namespace phoenix_service
{
    class microsvc_controller
    {
    public:
        microsvc_controller();
        ~microsvc_controller() = default;

        void start();
        void stop();

    private:
        boost::asio::io_service io_service_;
        std::unique_ptr<std::thread> thread_;
        std::unique_ptr<task_manager> task_manager_;
    };
}
