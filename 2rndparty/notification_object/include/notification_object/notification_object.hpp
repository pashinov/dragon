#ifndef NOTIFICATION_OBJECT_HPP
#define NOTIFICATION_OBJECT_HPP

#include "notification_object.h"

template <typename ...Args>
notification_handle notification_object<Args...>::connect(const notification_object::callback& f) const
{
    return handle(*storage_.add(f));
}

template <typename ...Args>
template <typename Object, typename Callback>
notification_handle notification_object<Args...>::connect(const Object& object, const Callback& callback) const
{
    // using SFINAE to validate templates arguments
    static_assert(std::is_invocable_v<Callback, Object*, Args...>, "Error, callback is not invocable with given arg");

    return connect(Callback([=](const Args&... args) { object->*call(args...); }));
}

template <typename ...Args>
void notification_object<Args...>::notify(const Args&... args)
{
    storage_.notify(args...);
}

#endif // NOTIFICATION_OBJECT_HPP