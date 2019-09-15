#ifndef NOTIFICATION_HANDLE_H
#define NOTIFICATION_HANDLE_H

// internal
#include "notification_token.h"
#include "storage_action.h"

class notification_handle
{
public:
    explicit notification_handle(notification_token_ptr token);

    void disconnect();

private:
    notification_token_ptr token_;
};

#endif // NOTIFICATION_HANDLE_H