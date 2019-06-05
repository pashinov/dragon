#include <iostream>
#include <zmq.hpp>

#include "falcon.pb.h"
#include "service.hpp"

broker::service::service() : ctx_(true)
{
    conn_.insert(std::make_pair(falcon::service_id::CCLIENT_ID, "ipc:///tmp/cclient"));
    conn_.insert(std::make_pair(falcon::service_id::RCLIENT_ID, "ipc:///tmp/rclient"));
}

void broker::service::stop()
{
    ctx_ = false;
}

int broker::service::start()
{
    auto zsock_recv = [](zmq::socket_t& zsocket, zmq::message_t& identity, zmq::message_t& emptyfrm,
                         zmq::message_t& data) {
        if (zsocket.recv(&identity) && identity.more()
            && zsocket.recv(&emptyfrm) && emptyfrm.more()
            && zsocket.recv(&data) && !data.more())
        {
            return true;
        }
        return false;
    };

    auto zsock_send = [](zmq::socket_t& zsocket, zmq::message_t& identity, zmq::message_t& emptyfrm,
                         zmq::message_t& data) {
        zsocket.send(identity, ZMQ_SNDMORE);
        zsocket.send(emptyfrm, ZMQ_SNDMORE);
        zsocket.send(data);
    };

    // TODO: config file
    std::string ownurl  = "ipc:///tmp/router";
    int timeout         = 5000;

    falcon::request_t  request;
    falcon::response_t response;

    std::vector<zmq::pollitem_t> items;
    std::shared_ptr<zmq::context_t> zctx = std::make_shared<zmq::context_t>();

    zmq::socket_t zsocket_frontend(*zctx, zmq::socket_type::router);
    zsocket_frontend.bind(ownurl);

    items.push_back({ zsocket_frontend, 0, ZMQ_POLLIN, 0 });

    // need to get zmq socket by num
    std::map<size_t, zmq::socket_t&> items_num;

    // need to get zmq socket by name
    std::unordered_map<std::string, zmq::socket_t> zsocket_backend;

    // registration of backend sockets
    for (const auto& [id, url]: conn_)
    {
        zsocket_backend.insert(std::make_pair(url, zmq::socket_t(*zctx, zmq::socket_type::dealer)));
        zsocket_backend.at(url).connect(url);

        items.push_back({ zsocket_backend.at(url), 0 , ZMQ_POLLIN, 0 });
        items_num.insert(std::make_pair(items.size() - 1, std::ref(zsocket_backend.at(url))));
    }

    while(ctx_)
    {
        // polling frontend and backend sockets:
        // item[0] - frontend socket
        // item[1], items[...] - backend sockets
        zmq::poll(&items[0], items.size(), timeout);

        // forward requests from frontend socket
        if (items[0].revents & ZMQ_POLLIN)
        {
            zmq::message_t  identity;
            zmq::message_t  emptyfrm;
            zmq::message_t  message;

            if (!zsock_recv(zsocket_frontend, identity, emptyfrm, message))
            {
                continue;
            }

            std::string buf(static_cast<char*>(message.data()), message.size());
            if (request.ParseFromString(buf))
            {
                auto srv = request.srv_id();
                std::string url = get_connection(srv);

                if (!url.empty())
                {
                    zsock_send(zsocket_backend.at(url), identity, emptyfrm, message);
                }
            }
        }

        // forward responses from backend sockets
        for (uint32_t i = 1; i < items.size(); i++)
        {
            if (items[i].revents & ZMQ_POLLIN)
            {
                zmq::message_t  identity;
                zmq::message_t  emptyfrm;
                zmq::message_t  message;

                if (!zsock_recv(items_num.at(i), identity, emptyfrm, message))
                {
                    continue;
                }

                zsock_send(zsocket_frontend, identity, emptyfrm, message);
            }
        }
    }

    return 0;
}

std::string broker::service::get_connection(const std::uint32_t& srv_id)
{
    std::string url;
    if (conn_.find(srv_id) != conn_.end())
    {
        url = conn_.at(srv_id);
    }
    return url;
}
