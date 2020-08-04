// system includes
#include <fstream>
#include <iostream>

// 3rdpatry includes
#include <nlohmann/json.hpp>

// project includes
#include <utils/config.hpp>

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
                nlohmann::json data = nlohmann::json::parse(ifs);

                cfg_->application.name                                              = data["Config"]["Application"]["Name"].get<std::string>();
                cfg_->application.version                                           = data["Config"]["Application"]["Version"].get<std::string>();

                cfg_->system.logging.path                                           = data["Config"]["System"]["Logging"]["Path"].get<std::string>();
                cfg_->system.logging.level                                          = spdlog::level::from_str(data["Config"]["System"]["Logging"]["Level"].get<std::string>());

            }
            catch(std::exception& ex)
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
