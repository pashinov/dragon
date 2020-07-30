// system includes
#include <csignal>
#include <iostream>
#include <thread>

// 3rd party includes
#include <args.hxx>
#include <boost/asio.hpp>

namespace asio = boost::asio;

static bool alive = false;

void service_main_thread(asio::io_service& io_service)
{
    io_service.run();
}

void signals_callback_handler(int signal)
{
    switch(signal) {
        case SIGINT:
        case SIGTERM:
        default:
            alive = false;
            break;
    }
}

void run_service()
{
    asio::io_service io_service;
    std::thread thread(&service_main_thread, std::ref(io_service));

    signal(SIGINT, signals_callback_handler);
    signal(SIGTERM, signals_callback_handler);

    alive = true;
    while(alive)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    io_service.stop();
    thread.join();
}

int main(int argc, char* argv[])
{
    // Parse arguments
    args::ArgumentParser parser("Dragon");
    args::HelpFlag help(parser, "help", "Display help menu", {'h', "help"});
    args::ValueFlag<std::string> config(parser, "config", "Configuration path", {'c'});
    args::Flag daemon(parser, "daemon", "Run as daemon", {'d'});
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

    // Start service
    run_service();

    return EXIT_SUCCESS;
}
