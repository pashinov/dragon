#ifndef BASE_NOTIFICATION_H
#define BASE_NOTIFICATION_H

// notification
#include <NotificationObject>

// std
#include <optional>
#include <iostream>

namespace ptree
{
    template <typename Traits>
    class base_notification
    {
    public:
        const notification_object<typename Traits::key_t>& child_added() const;
        const notification_object<typename Traits::key_t>& child_removed() const;
        const notification_object<std::optional<typename Traits::value_t> >& value_changed() const;

        base_notification(const base_notification& other) = default;
        base_notification(base_notification&& other) = default;

        base_notification& operator=(const base_notification& other) = default;
        base_notification& operator=(base_notification&& other) = default;

    protected:
        base_notification(const typename Traits::node_shared_ptr& node);
        ~base_notification() { }

    private:
        typename Traits::node_shared_ptr node_;
    };
}

#endif // BASE_NOTIFICATION_H

#include "base_notification.hpp"
