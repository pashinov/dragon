//
// Created by apashinov on 04/08/20
//

#pragma once

// system includes
#include <string>

// 3rdparty includes
#include <spdlog/spdlog.h>

namespace utils
{
    struct config_st
    {
        struct Application
        {
            std::string name;
            std::string version;
        };

        struct System
        {
            struct Logging
            {
                spdlog::level::level_enum level;
                std::string path;
            };

            Logging logging;
        };

        Application application;
        System system;
    };
}
