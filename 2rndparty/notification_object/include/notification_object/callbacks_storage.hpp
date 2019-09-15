#ifndef CALLBACKS_STORAGE_HPP
#define CALLBACKS_STORAGE_HPP

// internal
#include "callbacks_storage.h"

// std
#include <list>

template <typename ...Args>
callbacks_storage<Args...>::~callbacks_storage()
{
    for(auto it = callbacks_.begin(); it != callbacks_.end();)
    {
        notification_token_ptr token = it->first;
        it = callbacks_.erase(it);
        delete token;
    }
}

template <typename ...Args>
notification_token_ptr callbacks_storage<Args...>::add(const callbacks_storage::Callback& callback)
{
    notification_token_ptr token = new notification_token(this);
    callbacks_.emplace(token, callback);
    return token;
}

template <typename ...Args>
void callbacks_storage<Args...>::remove(notification_token_ptr token)
{
    if (token)
    {
        callbacks_.erase(token);
        delete token;
    }
}

template <typename ...Args>
void callbacks_storage<Args...>::notify(const Args& ...args)
{
    for (const auto& item : callbacks_)
    {
        item.second(args...);
    }
}

#endif // CALLBACKS_STORAGE_HPP