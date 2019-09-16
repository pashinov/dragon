#include <memory>

#include "service.hpp"

int main(int arcv, char* argv[])
{
    // TODO: daemonize
    auto srv = std::make_unique<sysinfo_svc::service>();
    return srv->start();
}
