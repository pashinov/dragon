#ifndef NOTIFICATION_OBJECT_HPP
#define NOTIFICATION_OBJECT_HPP

#include "notification_object.h"

template <typename ...Args>
notification_handle notification_object<Args...>::connect(const notification_object::callback& f) const
{
    return { };
}

template <typename ...Args>
void notification_object<Args...>::notify(const Args&... args)
{
}

#endif // NOTIFICATION_OBJECT_HPP