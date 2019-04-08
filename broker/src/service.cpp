#include <iostream>
#include <zmq.hpp>

#include "amp.pb.h"
#include "service.hpp"

broker::service::service() : ctx_(true)
{
    conn_storage_.insert(std::make_pair(amp::service_id::CCLIENT_ID, "ipc:///tmp/cclient"));
    //conn_storage_.insert(std::make_pair(amp::service_id::PCLIENT_ID, "ipc:///tmp/pclient"));
    //conn_storage_.insert(std::make_pair(amp::service_id::GCLIENT_ID, "ipc:///tmp/gclient"));
    //conn_storage_.insert(std::make_pair(amp::service_id::RCLIENT_ID, "ipc:///tmp/rclient"));
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
    std::string ownurl  = "ipc:///tmp/broker";
    int timeout         = 5000;

    amp::request_t  request;
    amp::response_t response;

    std::vector<zmq::pollitem_t> items;
    std::shared_ptr<zmq::context_t> zctx = std::make_shared<zmq::context_t>();

    zmq::socket_t zsocket_frontend(*zctx, zmq::socket_type::router);
    zsocket_frontend.bind(ownurl);

    items.push_back({ zsocket_frontend, 0, ZMQ_POLLIN, 0 });

    std::map<size_t, zmq::socket_t&> items_num;
    std::unordered_map<std::string, zmq::socket_t> zsocket_backend;
    for (const auto& it : conn_storage_)
    {
        std::string url = it.second;

        zsocket_backend.insert(std::make_pair(url, zmq::socket_t(*zctx, zmq::socket_type::dealer)));
        zsocket_backend.at(url).connect(url);

        items.push_back({ zsocket_backend.at(url), 0 , ZMQ_POLLIN, 0 });
        items_num.insert(std::make_pair(items.size() - 1, std::ref(zsocket_backend.at(url))));
    }

    while(ctx_)
    {
        zmq::poll(&items[0], items.size(), timeout);

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
    if (conn_storage_.find(srv_id) != conn_storage_.end())
    {
        url = conn_storage_.at(srv_id);
    }
    return url;
}