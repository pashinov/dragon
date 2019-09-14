#ifndef NOTIFICATION_OBJECT_H
#define NOTIFICATION_OBJECT_H

// internal
#include "notification_handle.h"

// std
#include <functional>

template <typename ...Args>
class notification_object
{
public:
    using callback = std::function<void(const Args&...)>;

public:
    notification_handle connect(const callback& f) const;
    void notify(const Args& ... args);
};

#endif // NOTIFICATION_OBJECT_H

#include "notification_object.hpp"