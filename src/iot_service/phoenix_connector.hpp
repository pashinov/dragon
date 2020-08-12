//
// Created by Alexey Pashinov on 05/08/20
//

#pragma once

// 3rdparty includes
#include <zmq.hpp>

namespace iot_service
{
    class phoenix_connector
    {
    public:
        phoenix_connector();
        ~phoenix_connector();

        void bind(const std::string& addr);
        void publish(const std::string& topic, const std::string& msg);

    private:
        zmq::context_t zmq_ctx_;
        zmq::socket_t  zmq_publisher_;
    };
}
