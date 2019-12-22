#ifndef BASE_NOTIFICATION_HPP
#define BASE_NOTIFICATION_HPP

// std
#include <exception>
#include <cassert>
#include <memory>

namespace ptree
{
    template <typename Traits>
    base_notification<Traits>::base_notification(const typename Traits::node_weak_ptr& node) : node_(node)
    {
        assert(node_.lock());
    }

    template <typename Traits>
    const notification_object<typename Traits::key_t>& base_notification<Traits>::child_added() const
    {
        const notification_object<typename Traits::key_t>& res
                = node_.lock()->child_added();
        return res;
    }

    template <typename Traits>
    const notification_object<typename Traits::key_t>& base_notification<Traits>::child_removed() const
    {
        const notification_object<typename Traits::key_t>& res
                = node_.lock()->child_removed();
        return res;
    }

    template <typename Traits>
    const notification_object<std::optional<typename Traits::value_t> >& base_notification<Traits>::value_changed() const
    {
        const notification_object<std::optional<typename Traits::value_t> >& res
                = node_.lock()->value_changed();
        return res;
    }
}
#endif // BASE_NOTIFICATION_HPP
