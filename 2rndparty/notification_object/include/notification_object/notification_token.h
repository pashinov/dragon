#ifndef NOTIFICATION_TOKEN_H
#define NOTIFICATION_TOKEN_H

// forward declaration class
class storage_action;
using storage_action_ptr = storage_action*;

class notification_token
{
public:
    explicit notification_token(storage_action_ptr storage);

    void reset(storage_action_ptr storage);

    [[nodiscard]]
    storage_action_ptr get() const;

private:
    storage_action_ptr storage_;
};

using notification_token_ptr = notification_token*;

#endif // NOTIFICATION_TOKEN_H
