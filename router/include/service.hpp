//
// Created by apashinov on 08/04/19
//

#ifndef FALCON_ROUTER_SERVICE_HPP
#define FALCON_ROUTER_SERVICE_HPP

// system includes
#include <unordered_map>

namespace router
{
    class service
    {
    public:
        service() : ctx_(true) {};
        ~service() = default;

        int start();
        void stop();

    private:
        // no copy
        service(const service&) = delete;
        service& operator=(const service&) = delete;

        // no move
        // no move
        service(service&&) = delete;
        service& operator=(service&&) = delete;

        // get service's url
        std::string get_connection(const std::uint32_t& svc_id, const std::unordered_map<std::uint32_t, std::string>& conn);

        // context
        bool ctx_;
    };
}

#endif // FALCON_ROUTER_SERVICE_HPP
