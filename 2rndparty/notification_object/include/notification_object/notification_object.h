#ifndef NOTIFICATION_OBJECT_H
#define NOTIFICATION_OBJECT_H

// internal
#include "notification_handle.h"
#include "storage_interface.h"
#include "objects_storage.h"

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
    notification_handle connect(const Object& object, const Callback& callback) const;

    void notify(const Args& ... args);

private:
    objects_storage<Args...> storage_;
};

#endif // NOTIFICATION_OBJECT_H

#include "notification_object.hpp"