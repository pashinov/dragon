#ifndef CALLBACKS_STORAGE_H
#define CALLBACKS_STORAGE_H

// internal
#include "storage_action.h"

// std
#include <functional>
#include <map>

template <typename ...Args>
class callbacks_storage : public storage_action
{
public:
    using Callback = std::function<void(const Args& ...args)>;

public:
    ~callbacks_storage() override;

    notification_token_ptr add(const Callback& callback);
    void notify(const Args&... args);

    void remove(notification_token_ptr token) override;

private:
    std::map<notification_token_ptr, Callback> callbacks_;
};

#endif // CALLBACKS_STORAGE_H

#include "callbacks_storage.hpp"