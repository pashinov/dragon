// system includes
#include <csignal>
#include <iostream>

// 3rd party includes
#include <args.hxx>

// project includes
#include <rest_service/microsvc_controller.hpp>
#include <utils/config.hpp>
#include <utils/interrupt_handler.hpp>
#include <utils/logger.hpp>

#include <version.hpp>

void run_service()
{
    rest_service::microservice_controller server;

    try
    {
        server.set_endpoint(CONFIG()->service.rest.endpoint);
        server.accept().wait();

        utils::interrupt_handler::hook_signal(SIGINT);
        utils::interrupt_handler::hook_signal(SIGTERM);
        utils::interrupt_handler::wait_for_signal_interrupt();

        server.shutdown().wait();
    }
    catch(std::exception& ex)
    {
        LOG_ERROR(LOGGER(CONFIG()->application.name), "Run service exception: {}", ex.what());
    }
}

int main(int argc, char* argv[])
{
    // Parse arguments
    args::ArgumentParser parser("Dragon");
    args::HelpFlag help                 (parser, "help",    "Display help menu",    {'h', "help"});
    args::Flag version                  (parser, "version", "Project version",      {'v', "version"});
    args::ValueFlag<std::string> config (parser, "config",  "Configuration path",   {'c'});
    args::Flag daemon                   (parser, "daemon",  "Run as daemon",        {'d'});
    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Help&)
    {
        std::cout << parser;
        return EXIT_SUCCESS;
    }
    catch (args::ParseError &ex)
    {
        std::cerr << ex.what() << std::endl;
        std::cerr << parser;
        return EXIT_FAILURE;
    }
    catch (args::ValidationError &ex)
    {
        std::cerr << ex.what() << std::endl;
        std::cerr << parser;
        return EXIT_FAILURE;
    }

    if (version)
    {
        std::cout << PROJECT_VERSION << std::endl;
        return EXIT_SUCCESS;
    }

    // Load configuration
    std::string config_file;
    if(config)
    {
        config_file = config.Get();
    }
    else
    {
        config_file = "/etc/dragon/dragon.json";
    }
    if(!utils::config::get_instance().load_config(config_file))
    {
        std::cerr << "Loading configuration" << std::endl;
        return EXIT_FAILURE;
    }

    // Init logging
    LOGGER(CONFIG()->application.name, CONFIG()->system.logging.level);

    // Start service
    run_service();

    return EXIT_SUCCESS;
}
