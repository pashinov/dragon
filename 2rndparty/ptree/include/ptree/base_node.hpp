#ifndef NODE_HPP
#define NODE_HPP

#include "base_node.h"

namespace ptree
{
    template <typename Traits>
    base_node<Traits>::base_node(const typename Traits::key_t& key, base_node<Traits>* parent)
        : key_(key), parent_(parent) { }

    template <typename Traits>
    const typename Traits::key_t& base_node<Traits>::key() const { return key_; }

    template <typename Traits>
    typename Traits::value_t base_node<Traits>::value() const { return value_.value(); }

    template <typename Traits>
    typename Traits::children_t& base_node<Traits>::children() const { return children_; }

    template <typename Traits>
    base_node<Traits>* base_node<Traits>::child(const typename Traits::key_t& key)
    {
        if (auto it = children_.find(key); it != children_.cend())
        {
            return it.second;
        }
        return nullptr;
    }

} //namespace

#endif //NODE_HPP