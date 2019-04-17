//
// Created by apashinov on 08/04/19
//

#ifndef AMP_CCLIENT_SERVICE_HPP
#define AMP_CCLIENT_SERVICE_HPP

#include <iomanip>
#include <fstream>
#include <nlohmann/json.hpp>

#include "amp.pb.h"

namespace cclient
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

        void set_to_storage(const amp::request_t& req, amp::response_t& resp);
        void print_storage(const amp::request_t& req, amp::response_t& resp);

        void pwr_led_1(const amp::request_t& req, amp::response_t& resp);

        void read_json(const std::string& file) { std::ifstream i(file); i >> json_; i.close(); }
        void write_json(const std::string& file) { std::ofstream o(file); o << std::setw(4) << json_ << std::endl; o.close(); }

        // context
        bool ctx_;

        // handlers of commands
        std::map<std::string, std::function<void(const amp::request_t& req, amp::response_t& resp)>> handler_;

        // storage
        nlohmann::json json_;
    };
}

#endif // AMP_CCLIENT_SERVICE_HPP
