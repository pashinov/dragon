#ifndef TRAITS_H
#define TRAITS_H

// std
#include <map>

namespace ptree
{
    // forward declaration node class
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
    };
} // namespace

#endif // TRAITS_H