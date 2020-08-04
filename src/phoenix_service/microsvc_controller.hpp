//
// Created by Alexey Pashinov on 3/08/20
//

#pragma once

// system includes
#include <thread>

// 3rdparty includes
#include <boost/asio.hpp>

namespace phoenix_service
{
    class microsvc_controller
    {
    public:
        microsvc_controller() = default;
        ~microsvc_controller() = default;

        void start();
        void stop();

    private:
        static void make_get_request(const std::string& endpoint, const std::string& uri);

    private:
        std::thread thread_;
        boost::asio::io_service io_service_;
    };
}
