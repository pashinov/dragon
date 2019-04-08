//
// Created by apashinov on 08/04/19
//

#ifndef AMP_BROKER_SERVICE_HPP
#define AMP_BROKER_SERVICE_HPP

#include <unordered_map>

namespace broker
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

        // get service's url
        std::string get_connection(const std::uint32_t& srv_id);

        // context
        bool ctx_;

        // storage of service's urls
        std::unordered_map<std::uint32_t, std::string> conn_storage_;
    };
}

#endif // AMP_BROKER_SERVICE_HPP
