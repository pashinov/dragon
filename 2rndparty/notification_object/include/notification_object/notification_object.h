#ifndef NOTIFICATION_OBJECT_H
#define NOTIFICATION_OBJECT_H

// internal
#include "notification_handle.h"
#include "storage_action.h"
#include "callbacks_storage.h"

// std
#include <functional>

template <typename ...Args>
class notification_object
{
public:
    using callback = std::function<void(const Args&...)>;

public:
    notification_handle connect(const callback& f) const;

    template <typename Object, typename Callback>
    notification_handle connect(Object* object, const Callback& callback) const;

    void notify(const Args& ... args);

private:
    mutable callbacks_storage<Args...> storage_;
};

#endif // NOTIFICATION_OBJECT_H

// implementation
#include "notification_object.hpp"
