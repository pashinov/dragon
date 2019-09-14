#ifndef NOTIFICATION_HANDLE_H
#define NOTIFICATION_HANDLE_H

// internal
#include "notification_token.h"
#include "storage_interface.h"

class notification_handle
{
public:
    explicit notification_handle(const notification_token& token) : token_(token) { }

    void disconnect()
    {
        token_.get()->remove(token_);
    }

private:
    notification_token token_;
};

#endif // NOTIFICATION_HANDLE_H