// 3rdparty includes
#include <cpprest/http_client.h>

// project includes
#include <phoenix_service/microsvc_controller.hpp>
#include <phoenix_service/task_manager.hpp>
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
                make_get_request("http://localhost:9001/api/v1", "cpuinfo");
            };

            std::function<void()> osinfo = [this]() {
                make_get_request("http://localhost:9001/api/v1", "osinfo");
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

    void microsvc_controller::make_get_request(const std::string& endpoint, const std::string& uri)
    {
        auto client = client::http_client{endpoint};
        auto response = client.request(http::methods::GET, uri).get();
        if (response.status_code() == status_codes::OK)
        {
            auto value = response.extract_json().get();
            try
            {
                LOG_INFO(LOGGER(CONFIG()->application.name), "{}", value.serialize());
            }
            catch (http_exception const& ex)
            {
                LOG_ERROR(LOGGER(CONFIG()->application.name), "Make GET request exception: {}", ex.what());
            }
        }
    }
}
