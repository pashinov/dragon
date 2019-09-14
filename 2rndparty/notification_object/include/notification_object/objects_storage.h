#ifndef OBJECTS_STORAGE_H
#define OBJECTS_STORAGE_H

// internal
#include "storage_interface.h"

// std
#include <functional>

template <typename ...Args>
class objects_storage : public storage_interface
{
public:
    using Callback = std::function<void(const Args& ...args)>;
public:
    notification_token_ptr add(const Callback& callback);
    void remove(const notification_token& token) override;
    void notify(const Args&... args);
};

#endif // OBJECTS_STORAGE_H

#include "objects_storage.hpp"