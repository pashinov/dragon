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

    std::optional<size_t> phoenix_connector::send(const std::string& msg)
    {
        zmq::message_t zmsg(std::begin(msg), std::end(msg));
        return zmq_publisher_.send(zmsg, zmq::send_flags::dontwait);
    }


    phoenix_connector::~phoenix_connector()
    {
        zmq_publisher_.close();
    }
}
