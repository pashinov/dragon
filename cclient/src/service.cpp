#include <zmq_addon.hpp>

#include "service.hpp"
#include "tpath/tpath.hpp"

void cclient::service::stop()
{
    ctx_ = false;
}

int cclient::service::start()
{
    zmq::context_t zctx;
    zmq::socket_t ssock(zctx, zmq::socket_type::router);

    zmq::multipart_t zmultipart_msg;

    // TODO: config file
    std::string url = "ipc:///tmp/cclient";
    int timeout     = 5000;

    // we bind the proxy socket
    ssock.bind(url);

    amp::request_t  packet;
    amp::response_t response;

    std::vector<zmq::pollitem_t> poller = { { ssock, 0, ZMQ_POLLIN, 0 } };

    while(ctx_)
    {
        zmq::poll(poller, timeout);

        if (poller[0].revents & ZMQ_POLLIN)
        {
            if (zmultipart_msg.recv(ssock))
            {
                std::string buf = zmultipart_msg.peekstr(zmultipart_msg.size() - 1);
                if (packet.ParseFromString(buf))
                {
                    auto cmd = packet.cmd_id();
                    switch(cmd)
                    {
                        case amp::command_id::CMD_SET_ID:
                        {
                            set(packet, response);
                            break;
                        }

                        case amp::command_id::CMD_PRINT_ID:
                        {
                            print(packet, response);
                            break;
                        }

                        default:
                            break;

                    }
                }

                response.set_srv_id(packet.srv_id());
                response.set_cmd_id(packet.cmd_id());
                auto rspstr = response.SerializeAsString();

                // replace last element that contain data with created response
                zmultipart_msg.remove();
                zmultipart_msg.add(zmq::message_t(std::begin(rspstr), std::end(rspstr)));

                zmultipart_msg.send(ssock);
            }
        }
    }

    return 0;
}

void cclient::service::set(const amp::request_t& req, amp::response_t& resp)
{
    int err = 0;

    tpath path = req.branch();

    std::vector<nlohmann::detail::iter_impl<nlohmann::basic_json<>>> it(path.elements_count());

    // json struct traversal: get node of path
    for (int i = 0; i < path.elements_count(); i++)
    {
        if (i == 0)
        {
            it[i] = json_.find(path[i]);
            if (it[i] == json_.end())
            {
                err = -1;
                break;
            }
        }
        else
        {
            it[i] = it[i-1].value().find(path[i]);
            if (it[i] == it[i-1].value().end())
            {
                err = -1;
                break;
            }
        }
    }

    // set value if node exist
    if (err == 0 && !it[path.elements_count()-1]->is_structured())
    {
        it[path.elements_count()-1].value() = req.payload();
        write_json("data.json");
        resp.set_status(0);
    }
    else
    {
        resp.set_status(1);
    }
}

void cclient::service::print(const amp::request_t& req, amp::response_t& resp)
{
    int err = 0;

    tpath path = req.branch();

    std::vector<nlohmann::detail::iter_impl<nlohmann::basic_json<>>> it(path.elements_count());

    // json struct traversal: get node of path
    for (int i = 0; i < path.elements_count(); i++)
    {
        if (i == 0)
        {
            it[i] = json_.find(path[i]);
            if (it[i] == json_.end())
            {
                err = -1;
                break;
            }
        }
        else
        {
            it[i] = it[i-1].value().find(path[i]);
            if (it[i] == it[i-1].value().end())
            {
                err = -1;
                break;
            }
        }
    }

    // write json.dump() to response if node exist
    if (err == 0)
    {
        resp.set_resp(it[path.elements_count()-1].value().dump());
        resp.set_status(0);
    }
    else
    {
        resp.set_status(1);
    }
}
