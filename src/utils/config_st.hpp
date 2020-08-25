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
                std::string endpoint;
            };

            struct IoT
            {
                struct Connector
                {
                    struct ZMQ
                    {
                        struct Pub
                        {
                            std::string addr;
                            std::string topic;
                        };

                        struct Sub
                        {
                            std::string addr;
                            std::string topic;
                            int timeout;
                        };

                        Pub pub;
                        Sub sub;
                    };

                    ZMQ zmq;
                };

                Connector connector;
            };

            REST rest;
            IoT iot;
        };

        Application application;
        System system;
        Service service;
    };
}
