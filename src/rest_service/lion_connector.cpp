// project includes
#include <rest_service//lion_connector.hpp>

namespace rest_service
{
    lion_connector::lion_connector()
    {
        zsock_ = zmq::socket_t(zctx_, ZMQ_REQ);
    }

    void lion_connector::connect(const std::string &addr)
    {
        std::lock_guard<std::mutex> guard(mutex_);
        zsock_.connect(addr);
    }

    void lion_connector::send(const std::string& identity, const std::string &msg)
    {
        std::lock_guard<std::mutex> guard(mutex_);

        zmq::message_t zid(std::begin(identity), std::end(identity));
        zsock_.send(zid, zmq::send_flags::sndmore);

        zmq::message_t zmsg(std::begin(msg), std::end(msg));
        zsock_.send(zmsg, zmq::send_flags::none);
    }

    void lion_connector::recv(std::string& identity, std::string &msg)
    {
        std::lock_guard<std::mutex> guard(mutex_);

        zmq::message_t zid;
        auto recv_identity_bytes = zsock_.recv(zid, zmq::recv_flags::none);
        if (!recv_identity_bytes.has_value() || recv_identity_bytes.value() <= 0)
        {
            return;
        }
        identity = std::string(static_cast<char*>(zid.data()), zid.size());

        zmq::message_t zmsg;
        auto recv_message_bytes = zsock_.recv(zmsg, zmq::recv_flags::none);
        if (!recv_message_bytes.has_value() || recv_message_bytes.value() <= 0)
        {
            return;
        }
        msg = std::string(static_cast<char*>(zmsg.data()), zmsg.size());
    }

    void lion_connector::disconnect(const std::string &addr)
    {
        std::lock_guard<std::mutex> guard(mutex_);
        if (zsock_.connected())
        {
            zsock_.disconnect(addr);
        }
    }

    bool lion_connector::is_connected()
    {
        return zsock_.connected();
    }
}
