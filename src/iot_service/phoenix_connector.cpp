// project includes
#include <iot_service/phoenix_connector.hpp>
#include <utils/config.hpp>
#include <utils/logger.hpp>

// proto
#include <phoenix.pb.h>

namespace iot_service
{
    phoenix_connector::phoenix_connector()
        : publisher_(std::make_unique<phoenix_connector::publisher>())
        , subscriber_(std::make_unique<phoenix_connector::subscriber>())
    {

    }

    phoenix_connector::publisher* phoenix_connector::publisher_instance()
    {
        return publisher_.get();
    }

    phoenix_connector::subscriber* phoenix_connector::subscriber_instance()
    {
        return subscriber_.get();
    }

    phoenix_connector::publisher::publisher()
    {
        zmq_publisher_ = zmq::socket_t(zmq_ctx_, ZMQ_PUB);
    }

    phoenix_connector::publisher::~publisher()
    {
        zmq_publisher_.close();
    }

    void phoenix_connector::publisher::bind(const std::string& addr)
    {
        zmq_publisher_.bind(addr);
    }

    void phoenix_connector::publisher::publish(const std::string& topic, const std::string& msg)
    {
        zmq::message_t ztopic(std::begin(topic), std::end(topic));
        zmq_publisher_.send(ztopic, zmq::send_flags::sndmore);
        zmq::message_t zmsg(std::begin(msg), std::end(msg));
        zmq_publisher_.send(zmsg, zmq::send_flags::dontwait);
    }

    phoenix_connector::subscriber::subscriber()
    {
        zmq_subscriber_ = zmq::socket_t(zmq_ctx_, ZMQ_SUB);
    }

    phoenix_connector::subscriber::~subscriber()
    {
        zmq_subscriber_.close();
    }

    void phoenix_connector::subscriber::connect(const std::string& addr)
    {
        zmq_subscriber_.connect(addr);
    }

    void phoenix_connector::subscriber::subscribe(const std::string& topic)
    {
        zmq_subscriber_.setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.length());
    }

    void phoenix_connector::subscriber::polling_loop(std::atomic<bool>& alive)
    {
        std::vector<zmq::pollitem_t> items = {{zmq_subscriber_, 0, ZMQ_POLLIN, 0}};
        while (alive)
        {
            int rc = 0;
            zmq::message_t topic;
            zmq::message_t msg;
            zmq::poll(&items[0], items.size(), CONFIG()->service.iot.connector.zmq.sub.timeout);

            if (items[0].revents & ZMQ_POLLIN)
            {
                try
                {
                    rc = zmq_subscriber_.recv(topic, zmq::recv_flags::none).value();
                    rc = zmq_subscriber_.recv(msg, zmq::recv_flags::none).value() && rc;
                    if(rc > 0)
                    {
                        phoenix_proto::message proto_msg;
                        if (proto_msg.ParseFromString(std::string(static_cast<char*>(msg.data()), msg.size())))
                        {
                            received_message_.push(std::make_pair(proto_msg.topic(), proto_msg.payload()));
                        }
                    }
                }
                catch (std::exception& ex)
                {
                    LOG_WARN(LOGGER(CONFIG()->application.name), "Receive zmq message failed: {}", ex.what());
                }
            }
        }
    }

    std::optional<std::pair<std::string, std::string>> phoenix_connector::subscriber::try_pop()
    {
        std::optional<std::pair<std::string, std::string>> data = std::nullopt;

        if (!received_message_.empty())
        {
            data = received_message_.front();
            received_message_.pop();
        }

        return data;
    }
}
