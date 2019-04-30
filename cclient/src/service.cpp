#include <zmq_addon.hpp>

#include "service.hpp"

cclient::service::service() : ctx_(true)
{
}

void cclient::service::stop()
{
    ctx_ = false;
}

int cclient::service::start()
{
    zmq::context_t zctx;
    zmq::socket_t ssock(zctx, zmq::socket_type::router);

    zmq::multipart_t zmsg;

    // TODO: config file
    std::string url = "ipc:///tmp/cclient";
    int timeout     = 5000;

    // bind socket
    ssock.bind(url);

    std::vector<zmq::pollitem_t> poller = { { ssock, 0, ZMQ_POLLIN, 0 } };

    while(ctx_)
    {
        zmq::poll(poller, timeout);

        if (poller[0].revents & ZMQ_POLLIN)
        {
            if (zmsg.recv(ssock))
            {
                amsp::request_t  packet;
                amsp::response_t response;

                std::string buf = zmsg.peekstr(zmsg.size() - 1);
                if (packet.ParseFromString(buf))
                {
                    response.set_resp("cclient: Hello, World!!");
                    response.set_status(0);
                }

                response.set_srv_id(packet.srv_id());
                response.set_cmd_id(packet.cmd_id());
                response.set_branch(packet.branch());

                auto rspstr = response.SerializeAsString();

                // replace last element that contains data with created response
                zmsg.remove();
                zmsg.add(zmq::message_t(std::begin(rspstr), std::end(rspstr)));

                zmsg.send(ssock);
            }
        }
    }

    return 0;
}
