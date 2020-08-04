//
//  Created by Ivan Mejia on 12/03/16.
//
// MIT License
//
// Copyright (c) 2016 ivmeroLabs. All rights reserved.
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
#include <rest_service/foundation/include/basic_controller.hpp>

using namespace web;
using namespace http;
using namespace experimental::listener;

namespace rest_service
{
    basic_controller::basic_controller() = default;
    basic_controller::~basic_controller() = default;

    void basic_controller::set_endpoint(const std::string& value)
    {
        uri endpoint_uri(value);
        uri_builder endpoint_builder;

        endpoint_builder.set_scheme(endpoint_uri.scheme());
        endpoint_builder.set_host(endpoint_uri.host());
        endpoint_builder.set_port(endpoint_uri.port());
        endpoint_builder.set_path(endpoint_uri.path());

        listener_ = http_listener(endpoint_builder.to_uri());
    }

    std::string basic_controller::endpoint() const
    {
        return listener_.uri().to_string();
    }

    pplx::task<void> basic_controller::accept()
    {
        init_rest_handlers();
        return listener_.open();
    }

    pplx::task<void> basic_controller::shutdown()
    {
        return listener_.close();
    }

    std::vector<utility::string_t> basic_controller::request_path(const http_request& message)
    {
        auto relative_path = uri::decode(message.relative_uri().path());
        return uri::split_path(relative_path);
    }
}
