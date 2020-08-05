// 3rdparty includes
#include <cpprest/http_client.h>

// project includes
#include <phoenix_service/microsvc_controller.hpp>
#include <phoenix_service/task_manager.hpp>
#include <system/sysinfo/cpuinfo.hpp>
#include <system/sysinfo/osinfo.hpp>
#include <utils/config.hpp>
#include <utils/logger.hpp>

using namespace web;
using namespace http;

namespace phoenix_service
{
    microsvc_controller::microsvc_controller()
    {
        task_manager_ = std::make_unique<task_manager>(io_service_);
    }

    void microsvc_controller::start()
    {
        thread_ = std::make_unique<std::thread>([this]() {
            std::function<void()> cpuinfo = [this]() {
                auto data = json::value::object();
                data["model"] = json::value::string(sys::sysinfo::cpu_model());
                data["vendor"] = json::value::string(sys::sysinfo::cpu_vendor());

                LOG_INFO(LOGGER(CONFIG()->application.name), "{}", data.serialize());
            };

            std::function<void()> osinfo = [this]() {
                auto data = json::value::object();
                data["name"] = json::value::string(sys::sysinfo::os_name());
                data["release"] = json::value::string(sys::sysinfo::os_release());
                data["version"] = json::value::string(sys::sysinfo::os_version());
                data["machine"] = json::value::string(sys::sysinfo::os_machine());
                data["system_name"] = json::value::string(sys::sysinfo::os_system_name());

                LOG_INFO(LOGGER(CONFIG()->application.name), "{}", data.serialize());
            };

            task_manager_->add_task(cpuinfo);
            task_manager_->add_task(osinfo);
            task_manager_->start();

            io_service_.run();
        });
    }

    void microsvc_controller::stop()
    {
        task_manager_->stop();
        io_service_.stop();
        thread_->join();
    }
}
