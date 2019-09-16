//
// Created by apashinov on 08/04/19
//

#ifndef FLACON_SYSINFO_SERVICE_HPP
#define FLACON_SYSINFO_SERVICE_HPP

// 3rdparty includes
#include <zmq.hpp>

// 2ndparty includes
#include <falcon.pb.h>

namespace sysinfo_svc
{
    class service
    {
    public:
        service();
        ~service() = default;

        int start();
        void stop();

    private:
        // no copy
        service(const service&) = delete;
        service& operator=(const service&) = delete;

        // no move
        service(service&&) = delete;
        service& operator=(service&&) = delete;

        // register service
        bool register_service(zmq::context_t& zctx, const std::string& reg_url, const std::string& dest_url);

        // deregister service
        bool deregister_service(zmq::context_t& zctx, const std::string& dereg_url, const std::string& dest_url);

        // context
        bool ctx_;
    };
}

#endif // FLACON_SYSINFO_SERVICE_HPP
