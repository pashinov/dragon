#ifndef STORAGE_ACTION_H
#define STORAGE_ACTION_H

// internal
#include "notification_token.h"

class storage_action
{
public:
    virtual ~storage_action() = default;

    virtual void remove(notification_token_ptr token) = 0;
};

#endif // STORAGE_ACTION_H