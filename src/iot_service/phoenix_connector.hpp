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
        std::optional<size_t> send(const std::string& msg);

    private:
        zmq::context_t zmq_ctx_;
        zmq::socket_t  zmq_publisher_;
    };
}
