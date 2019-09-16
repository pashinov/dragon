// 3rdparty includes
#include <zmq_addon.hpp>

// project includes
#include <service.hpp>

sysinfo_svc::service::service() : ctx_(true)
{
}

void sysinfo_svc::service::stop()
{
    ctx_ = false;
}

int sysinfo_svc::service::start()
{
    zmq::context_t zctx;
    zmq::socket_t socket(zctx, zmq::socket_type::router);

    zmq::multipart_t zmsg;

    // TODO: config file
    std::string url = "ipc:///tmp/sysinfo-svc";
    int timeout     = 5000;

    std::string router_url = "ipc:///tmp/router";

    // bind socket
    socket.bind(url);

    // register service
    if (!register_service(zctx, url, router_url))
    {
        throw std::runtime_error("sysinfo-svc: Unable to register");
    }

    std::vector<zmq::pollitem_t> poller = { { socket, 0, ZMQ_POLLIN, 0 } };

    while(ctx_)
    {
        zmq::poll(poller, timeout);

        if (poller[0].revents & ZMQ_POLLIN)
        {
            if (zmsg.recv(socket))
            {
                falcon::request_t  packet;
                falcon::response_t response;

                std::string buf = zmsg.peekstr(zmsg.size() - 1);
                if (packet.ParseFromString(buf))
                {
                    response.set_resp("sysinfo-svc response: " + packet.payload());
                    response.set_status(0);
                }

                response.set_svc_id(packet.svc_id());
                response.set_cmd_id(packet.cmd_id());
                response.set_branch(packet.branch());

                auto rspstr = response.SerializeAsString();

                // replace last element that contains data with created response
                zmsg.remove();
                zmsg.add(zmq::message_t(std::begin(rspstr), std::end(rspstr)));

                zmsg.send(socket);
            }
        }
    }

    // deregister service
    if (!deregister_service(zctx, url, router_url))
    {
        throw std::runtime_error("sysinfo-svc: Unable to deregister");
    }

    return 0;
}

bool sysinfo_svc::service::register_service(zmq::context_t& zctx, const std::string& reg_url, const std::string& dest_url)
{
    bool rval = false;

    falcon::reg_data_t reg_data;
    reg_data.set_svc_url(reg_url);

    falcon::request_t request;
    falcon::response_t response;

    request.set_svc_id(falcon::service_id::SYSINFO_SVC_ID);
    request.set_cmd_id(falcon::command_id::CMD_REG_ID);
    request.set_payload(reg_data.SerializeAsString());

    auto req_str = request.SerializeAsString();
    zmq::message_t req_msg(std::begin(req_str), std::end(req_str));

    zmq::socket_t socket(zctx, ZMQ_REQ);
    socket.connect(dest_url);

    socket.send(req_msg);

    zmq::message_t reply;
    socket.recv(&reply);

    std::string buf(static_cast<char*>(reply.data()), reply.size());

    if (response.ParseFromString(buf))
    {
        if (response.status() == 0)
        {
            rval = true;
        }
    }

    return rval;
}

bool sysinfo_svc::service::deregister_service(zmq::context_t& zctx, const std::string& dereg_url, const std::string& dest_url)
{
    bool rval = false;

    falcon::reg_data_t dereg_data;
    dereg_data.set_svc_url(dereg_url);

    falcon::request_t request;
    falcon::response_t response;

    request.set_cmd_id(falcon::command_id::CMD_DEREG_ID);
    request.set_payload(dereg_data.SerializeAsString());

    auto req_str = request.SerializeAsString();
    zmq::message_t req_msg(std::begin(req_str), std::end(req_str));

    zmq::socket_t socket(zctx, ZMQ_REQ);
    socket.connect(dest_url);

    socket.send(req_msg);

    zmq::message_t reply;
    socket.recv (&reply);

    std::string buf(static_cast<char*>(reply.data()), reply.size());

    if (response.ParseFromString(buf))
    {
        if (response.status() == 0)
        {
            rval = true;
        }
    }

    return rval;
}