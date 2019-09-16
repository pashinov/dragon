// system includes
#include <memory>

// 2ndparty includes
#include <logger/logger.hpp>

// project includes
#include <service.hpp>

int main(int arcv, char* argv[])
{
    // Logging initialization
    auto log = LOGGER("router", spdlog::level::info);
    LOG_INFO(log, "Starting application...");

    // TODO: daemonize
    auto srv = std::make_unique<sysinfo_svc::service>();
    return srv->start();
}
