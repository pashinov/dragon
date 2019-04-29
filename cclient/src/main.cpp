#include <memory>

#include "service.hpp"

int main(int arcv, char* argv[])
{
    // TODO: daemonize
    auto srv = std::make_unique<cclient::service>();
    return srv->start();
}
