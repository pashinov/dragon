//
//  Created by Ivan Mejia on 12/24/16.
//
// MIT License
//
// Copyright (c) 2016 ivmeroLabs.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

// project includes
#include <rest_service/microsvc_controller.hpp>
#include <system/sysinfo.hpp>

using namespace web;
using namespace http;

namespace rest_service
{
    void microservice_controller::init_rest_handlers()
    {
        listener_.support(methods::GET, [this](http_request message) { this->handle_get(message); });
    }

    void microservice_controller::handle_get(http_request message)
    {
        pplx::create_task([=]() {
            auto path = basic_controller::request_path(message);
            if (!path.empty())
            {
                if (path[0] == "sysinfo")
                {
                    auto response = json::value::object();

                    auto arch = sys::sysinfo::get(lion_protocol::SysInfoType::ARCH);
                    if (!arch.has_value())
                    {
                        message.reply(status_codes::InternalError);
                        return;
                    }

                    auto os = sys::sysinfo::get(lion_protocol::SysInfoType::OS);
                    if (!os.has_value())
                    {
                        message.reply(status_codes::InternalError);
                        return;
                    }

                    auto os_release = sys::sysinfo::get(lion_protocol::SysInfoType::OS_RELEASE);
                    if (!os_release.has_value())
                    {
                        message.reply(status_codes::InternalError);
                        return;
                    }

                    auto cpu_num = sys::sysinfo::get(lion_protocol::SysInfoType::CPU_NUM);
                    if (!cpu_num.has_value())
                    {
                        message.reply(status_codes::InternalError);
                        return;
                    }

                    auto cpu_speed = sys::sysinfo::get(lion_protocol::SysInfoType::CPU_SPEED);
                    if (!cpu_speed.has_value())
                    {
                        message.reply(status_codes::InternalError);
                        return;
                    }

                    auto storage_total = sys::sysinfo::get(lion_protocol::SysInfoType::STORAGE_TOTAL);
                    if (!storage_total.has_value())
                    {
                        message.reply(status_codes::InternalError);
                        return;
                    }

                    auto storage_free = sys::sysinfo::get(lion_protocol::SysInfoType::STORAGE_FREE);
                    if (!storage_free.has_value())
                    {
                        message.reply(status_codes::InternalError);
                        return;
                    }

                    auto uptime = sys::sysinfo::get(lion_protocol::SysInfoType::UPTIME);
                    if (!uptime.has_value())
                    {
                        message.reply(status_codes::InternalError);
                        return;
                    }

                    auto temperature = sys::sysinfo::get(lion_protocol::SysInfoType::TEMPERATURE);
                    if (!temperature.has_value())
                    {
                        message.reply(status_codes::InternalError);
                        return;
                    }

                    response["arch"] = json::value::string(std::get<std::string>(arch.value()));
                    response["os"] = json::value::string(std::get<std::string>(os.value()));
                    response["os_release"] = json::value::string(std::get<std::string>(os_release.value()));
                    response["cpu_num"] = json::value::number(std::get<std::uint32_t>(cpu_num.value()));
                    response["cpu_speed"] = json::value::number(std::get<std::uint32_t>(cpu_speed.value()));
                    response["storage_total"] = json::value::number(std::get<std::uint32_t>(storage_total.value()));
                    response["storage_free"] = json::value::number(std::get<std::uint32_t>(storage_free.value()));
                    response["uptime"] = json::value::string(std::get<std::string>(uptime.value()));
                    response["temperature"] = json::value::number(std::get<double>(temperature.value()));

                    message.reply(status_codes::OK, response);
                }
                else
                {
                    message.reply(status_codes::NotFound);
                }
            }
            else
            {
                message.reply(status_codes::NotFound);
            }
        });
    }

    void microservice_controller::handle_put(web::http::http_request message)
    {
        message.reply(status_codes::NotImplemented, response_not_impl(methods::PUT));
    }

    void microservice_controller::handle_post(web::http::http_request message)
    {
        message.reply(status_codes::NotImplemented, response_not_impl(methods::POST));
    }

    void microservice_controller::handle_delete(web::http::http_request message)
    {
        message.reply(status_codes::NotImplemented, response_not_impl(methods::DEL));
    }

    void microservice_controller::handle_options(web::http::http_request message)
    {
        message.reply(status_codes::NotImplemented, response_not_impl(methods::OPTIONS));
    }

    void microservice_controller::handle_patch(web::http::http_request message)
    {
        message.reply(status_codes::NotImplemented, response_not_impl(methods::PATCH));
    }

    void microservice_controller::handle_head(web::http::http_request message)
    {
        message.reply(status_codes::NotImplemented, response_not_impl(methods::HEAD));
    }

    void microservice_controller::handle_trace(web::http::http_request message)
    {
        message.reply(status_codes::NotImplemented, response_not_impl(methods::TRCE));
    }

    void microservice_controller::handle_connect(web::http::http_request message)
    {
        message.reply(status_codes::NotImplemented, response_not_impl(methods::CONNECT));
    }

    void microservice_controller::handle_merge(web::http::http_request message)
    {
        message.reply(status_codes::NotImplemented, response_not_impl(methods::MERGE));
    }

    json::value microservice_controller::response_not_impl(const http::method& method)
    {
        auto response = json::value::object();
        response["serviceName"] = json::value::string("Dragon Microservice");
        response["http_method"] = json::value::string(method);
        return response;
    }
}
