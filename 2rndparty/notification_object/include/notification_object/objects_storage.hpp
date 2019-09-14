#ifndef OBJECTS_STORAGE_HPP
#define OBJECTS_STORAGE_HPP

// internal
#include "objects_storage.h"

template <typename ...Args>
notification_token_ptr objects_storage<Args...>::add(const objects_storage::Callback& callback)
{
    notification_token_ptr token = new notification_token(this);

    return token;
}

template <typename ...Args>
void objects_storage<Args...>::remove(const notification_token& token)
{
}

template <typename ...Args>
void objects_storage<Args...>::notify(const Args& ...args)
{

}

#endif // OBJECTS_STORAGE_HPP