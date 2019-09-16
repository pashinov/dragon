// 3rdparty includes
#include <zmq.hpp>

// 2ndparty includes
#include <falcon.pb.h>

// project includes
#include <service.hpp>

void router::service::stop()
{
    ctx_ = false;
}

int router::service::start()
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
    std::string zmq_url = "ipc:///tmp/router";
    int zmq_timeout     = 5000;

    falcon::request_t  request;
    falcon::response_t response;

    std::vector<zmq::pollitem_t> items;
    std::shared_ptr<zmq::context_t> zctx = std::make_shared<zmq::context_t>();

    zmq::socket_t zsocket_frontend(*zctx, zmq::socket_type::router);
    zsocket_frontend.bind(zmq_url);

    items.push_back({ zsocket_frontend, 0, ZMQ_POLLIN, 0 });

    // need to get zmq socket by num
    std::map<size_t, zmq::socket_t&> items_num;

    // need to get zmq socket by name
    std::unordered_map<std::string, zmq::socket_t> zsocket_backend;

    // storage of registered service's zmq url's
    std::unordered_map<std::uint32_t, std::string> connections;

    while(ctx_)
    {
        // polling frontend and backend sockets:
        // item[0] - frontend socket
        // item[1], items[...] - backend sockets
        zmq::poll(&items[0], items.size(), zmq_timeout);

        // forward requests from frontend socket
        if (items[0].revents & ZMQ_POLLIN)
        {
            int rval = -1;

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
                auto cmd = request.cmd_id();
                switch (cmd)
                {
                    case falcon::command_id::CMD_REG_ID:
                    {
                        falcon::reg_data_t reg_data;
                        if (reg_data.ParseFromString(request.payload()))
                        {
                            auto svc_id = request.svc_id();
                            auto svc_url = reg_data.svc_url();

                            connections.insert(std::make_pair(svc_id, svc_url));

                            zsocket_backend.insert(std::make_pair(svc_url, zmq::socket_t(*zctx, zmq::socket_type::dealer)));
                            zsocket_backend.at(svc_url).connect(svc_url);

                            items.push_back({ zsocket_backend.at(svc_url), 0 , ZMQ_POLLIN, 0 });
                            items_num.insert(std::make_pair(items.size() - 1, std::ref(zsocket_backend.at(svc_url))));

                            rval = 0;
                        }
                        break;
                    }

                    case falcon::command_id::CMD_DEREG_ID:
                    {
                        falcon::dereg_data_t dereg_data;
                        if (dereg_data.ParseFromString(request.payload()))
                        {
                            auto svc_id = request.svc_id();
                            auto svc_url = dereg_data.svc_url();

                            auto connit = connections.find(svc_id);
                            auto backit = zsocket_backend.find(svc_url);
                            if (connit != connections.end() && backit != zsocket_backend.end())
                            {
                                connections.erase(connit);

                                for (const auto& it : items_num)
                                {
                                    if (it.second == (*backit).second)
                                    {
                                        auto index = it.first;

                                        // we have to save size of items and we have not to remove
                                        // deleted socket from items otherwise we will have problems
                                        // with indexes of sockets
                                        items.at(index) = { nullptr, 0, 0 ,0 };

                                        items_num.erase(index);

                                        break;
                                    }
                                }

                                (*backit).second.close();
                                zsocket_backend.erase(backit);

                                rval = 0;
                            }
                        }
                        break;
                    }

                    default:
                    {
                        auto svc = request.svc_id();
                        std::string url = get_connection(svc, connections);

                        if (!url.empty())
                        {
                            zsock_send(zsocket_backend.at(url), identity, emptyfrm, message);
                            continue;
                        }
                        break;
                    }
                }
            }

            response.set_status(rval);

            auto resp_str = response.SerializeAsString();
            zmq::message_t resp_msg(std::begin(resp_str), std::end(resp_str));
            zsock_send(zsocket_frontend, identity, emptyfrm, resp_msg);
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

std::string router::service::get_connection(const std::uint32_t& svc_id, const std::unordered_map<std::uint32_t, std::string>& conn)
{
    std::string url;
    if (conn.find(svc_id) != conn.end())
    {
        url = conn.at(svc_id);
    }
    return url;
}
