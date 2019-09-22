// internal
#include "notification_object/notification_token.h"

notification_token::notification_token(storage_action_ptr storage) : storage_(storage) { }

notification_token::~notification_token() = default;

void notification_token::reset(storage_action_ptr storage)
{
    storage_ = storage;
}

storage_action_ptr notification_token::get() const { return storage_; }
