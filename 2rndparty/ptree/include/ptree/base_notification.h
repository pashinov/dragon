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
        const notification_object<typename Traits::key_t>& child_added() const;
        const notification_object<typename Traits::key_t>& child_removed() const;
        const notification_object<std::optional<typename Traits::value_t> >& value_changed() const;

    protected:
        base_notification(const typename Traits::node_weak_ptr& node);
        ~base_notification() = default;

    private:
        typename Traits::node_weak_ptr node_;
    };
}

#endif // BASE_NOTIFICATION_H

#include "base_notification.hpp"
