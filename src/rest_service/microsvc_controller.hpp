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

#pragma once 

#include <rest_service/foundation/include/basic_controller.hpp>

namespace rest_service
{
    class microservice_controller : public basic_controller, controller
    {
    public:
        microservice_controller() : basic_controller() {}
        ~microservice_controller() = default;
        void handle_get(web::http::http_request message) override;
        void handle_put(web::http::http_request message) override;
        void handle_post(web::http::http_request message) override;
        void handle_delete(web::http::http_request message) override;
        void handle_options(web::http::http_request message) override;
        void handle_patch(web::http::http_request message) override;
        void handle_head(web::http::http_request message) override;
        void handle_trace(web::http::http_request message) override;
        void handle_connect(web::http::http_request message) override;
        void handle_merge(web::http::http_request message) override;
        void init_rest_handlers() override;

    private:
        static web::json::value response_not_impl(const web::http::method& method);
    };
}
