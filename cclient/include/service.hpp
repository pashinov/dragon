//
// Created by apashinov on 08/04/19
//

#ifndef AMP_CCLIENT_SERVICE_HPP
#define AMP_CCLIENT_SERVICE_HPP

#include "falcon.pb.h"

namespace cclient
{
    class service
    {
    public:
        service();
        ~service() = default;

        int start();
        void stop();

    private:
        // no copy
        service(const service&) = delete;
        service& operator=(const service&) = delete;

        // context
        bool ctx_;
    };
}

#endif // AMP_CCLIENT_SERVICE_HPP
