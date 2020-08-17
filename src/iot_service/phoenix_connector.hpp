//
// Created by Alexey Pashinov on 05/08/20
//

#pragma once

// system includes
#include <queue>
#include <optional>

// 3rdparty includes
#include <zmq.hpp>

namespace iot_service
{
    class phoenix_connector
    {
    public:
        class publisher
        {
        public:
            publisher();
            ~publisher();

            void bind(const std::string& addr);
            void publish(const std::string& topic, const std::string& msg);
        private:
            zmq::context_t zmq_ctx_;
            zmq::socket_t  zmq_publisher_;
        };

        class subscriber
        {
        public:
            subscriber();
            ~subscriber();

            void connect(const std::string& addr);
            void subscribe(const std::string& topic);
            void polling_loop(std::atomic<bool>& alive);
            std::optional<std::pair<std::string, std::string>> try_pop();

        private:
            zmq::context_t zmq_ctx_;
            zmq::socket_t  zmq_subscriber_;
            std::queue<std::pair<std::string, std::string>> received_message_;
        };

        phoenix_connector();
        ~phoenix_connector() = default;

        publisher* publisher_instance();
        subscriber* subscriber_instance();

    private:
        std::unique_ptr<publisher> publisher_;
        std::unique_ptr<subscriber> subscriber_;
    };
}
