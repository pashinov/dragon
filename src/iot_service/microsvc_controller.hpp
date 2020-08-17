//
// Created by Alexey Pashinov on 03/08/20
//

#pragma once

// system includes
#include <thread>

// 3rdparty includes
#include <boost/asio.hpp>

// project includes
#include <iot_service/phoenix_connector.hpp>
#include <iot_service/task_manager.hpp>

namespace iot_service
{
    class microsvc_controller
    {
    public:
        microsvc_controller();
        ~microsvc_controller() = default;

        void start();
        void stop();

    private:
        std::atomic<bool> alive_;
        std::thread pub_thread_;
        std::thread sub_thread_;
        boost::asio::io_service io_service_;
        std::unique_ptr<task_manager> task_manager_;
        std::unique_ptr<phoenix_connector> phoenix_connector_;
    };
}
