#ifndef NOTIFICATION_TOKEN_H
#define NOTIFICATION_TOKEN_H

// forward declaration class
class storage_interface;
using storage_ptr = storage_interface*;

class notification_token
{
public:
    explicit notification_token(storage_ptr base) : base_(base) { }

    void reset(storage_ptr base) { base_ = base; }

    [[nodiscard]]
    storage_ptr get() const { return base_; }

private:
    storage_ptr base_;
};

using notification_token_ptr = notification_token*;

#endif // NOTIFICATION_TOKEN_H
