#ifndef TRAITS_H
#define TRAITS_H

// notification_object
#include <NotificationObject>

// std
#include <map>

namespace ptree
{
    // forward declaration base node class
    template <typename Traits>
    class base_node;

    template <typename Key, typename Value>
    class traits
    {
    public:
        using node_t = base_node<traits>;
        using node_ptr = node_t*;
        using key_t = Key;
        using value_t = Value;
        using children_t = std::map<key_t, node_ptr>;
        using value_changed_t = notification_object<value_t>;

    public:
        static node_ptr root();
    };

} // namespace

#endif // TRAITS_H

#include "traits.hpp"