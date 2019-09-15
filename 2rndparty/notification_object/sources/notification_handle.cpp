// internal
#include "notification_object/notification_handle.h"

// std
#include <cassert>

notification_handle::notification_handle(notification_token_ptr token) : token_(token) { assert(token_); }

void notification_handle::disconnect()
{
    if (token_) token_->get()->remove(token_);
}