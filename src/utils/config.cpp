// system includes
#include <iostream>

// 3rdpatry includes
#include <cpprest/json.h>

// project includes
#include <utils/config.hpp>

using namespace web;

namespace utils
{
    bool config::load_config(const std::string& filename) noexcept
    {
        int rval = true;

        std::ifstream ifs {filename};
        if(!ifs.fail())
        {
            try
            {
                json::value data = json::value::parse(ifs);

                cfg_->application.name                      = data.at("Config").at("Application").at("Name").as_string();
                cfg_->application.version                   = data.at("Config").at("Application").at("Version").as_string();

                cfg_->system.logging.path                   = data.at("Config").at("System").at("Logging").at("Path").as_string();
                cfg_->system.logging.level                  = spdlog::level::from_str(data.at("Config").at("System").at("Logging").at("Level").as_string());

                cfg_->service.rest.endpoint                 = data.at("Config").at("Service").at("REST").at("Endpoint").as_string();
                cfg_->service.iot.connector.zmq.pub.addr    = data.at("Config").at("Service").at("IoT").at("Connector").at("ZMQ").at("Pub").at("Addr").as_string();
                cfg_->service.iot.connector.zmq.pub.topic   = data.at("Config").at("Service").at("IoT").at("Connector").at("ZMQ").at("Pub").at("Topic").as_string();
            }
            catch(json::json_exception& ex)
            {
                std::cerr << ex.what() << std::endl;
                rval = false;
            }
        }
        else
        {
            rval = false;
        }

        return rval;
    }

    const config_st* config::get_config() noexcept
    {
        return cfg_.get();
    }
}
