#ifndef NOTIFICATION_BASE_H
#define NOTIFICATION_BASE_H

// internal
#include "notification_token.h"

// std
#include <functional>

class storage_interface
{
public:
    virtual ~storage_interface() = default;

    virtual void remove(const notification_token& token) = 0;
};

#endif // NOTIFICATION_BASE_H