// project includes
#include <iot_service/phoenix_connector.hpp>

namespace iot_service
{
    phoenix_connector::phoenix_connector()
    {
        zmq_publisher_ = zmq::socket_t(zmq_ctx_, ZMQ_PUB);
    }

    void phoenix_connector::bind(const std::string& addr)
    {
        zmq_publisher_.bind(addr);
    }

    void phoenix_connector::publish(const std::string& topic, const std::string& msg)
    {
        zmq::message_t ztopic(std::begin(topic), std::end(topic));
        zmq_publisher_.send(ztopic, zmq::send_flags::sndmore);
        zmq::message_t zmsg(std::begin(msg), std::end(msg));
        zmq_publisher_.send(zmsg, zmq::send_flags::dontwait);
    }

    phoenix_connector::~phoenix_connector()
    {
        zmq_publisher_.close();
    }
}
