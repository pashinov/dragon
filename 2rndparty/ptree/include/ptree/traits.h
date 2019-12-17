#ifndef PTREE_TRAITS_H
#define PTREE_TRAITS_H

// notification_object
#include <NotificationObject>

// std
#include <map>
#include <optional>
#include <memory>

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
        using optional_value_t = std::optional<value_t>;
    public:
        static node_ptr root();
    };

} // namespace

#endif // PTREE_TRAITS_H

#include "traits.hpp"
