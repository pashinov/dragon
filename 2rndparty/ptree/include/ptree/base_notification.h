#ifndef BASE_NOTIFICATION_H
#define BASE_NOTIFICATION_H

// notification
#include <NotificationObject>

// std
#include <optional>

namespace ptree
{
    template <typename Traits>
    class base_notification
    {
    public:
        base_notification(typename Traits::node_ptr node);
        virtual ~base_notification() = default;

        const notification_object<typename Traits::key_t>& child_added() const;
        const notification_object<typename Traits::key_t>& child_removed() const;
        const notification_object<std::optional<typename Traits::value_t> >& value_changed() const;

    private:
        typename Traits::node_ptr node_;
    };
}

#endif // BASE_NOTIFICATION_H

#include "base_notification.hpp"
