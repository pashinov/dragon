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

        struct Service
        {
            struct REST
            {
                struct Connector
                {
                    struct Lion
                    {
                        struct ZmqReq
                        {
                            std::string addr;
                        };

                        ZmqReq zmq_req;
                    };

                    Lion lion;
                };

                std::string endpoint;
                Connector connector;
            };

            REST rest;
        };


        Application application;
        System system;
        Service service;
    };
}
