#ifndef NODE_HPP
#define NODE_HPP

// internal
#include "base_node.h"
#include "base_node_error.h"

namespace ptree
{
    template <typename Traits>
    base_node<Traits>::base_node(const typename Traits::key_t& key, typename Traits::node_ptr parent)
        : key_(key), parent_(parent) { }

    template <typename Traits>
    base_node<Traits>::~base_node()
    {
        for(auto it = children_.begin(); it != children_.end(); ++it) { delete it->second; }
    }

    template <typename Traits>
    bool base_node<Traits>::set(const typename Traits::value_t& value)
    {
        bool was_set = false;
        if (holds_value_ == holds_value_t::child) was_set = false;
        else
        {
            value_.reset();
            value_.emplace(value);
            holds_value_ = holds_value_t::value;
            was_set = true;
        }
        return was_set;
    }

    template <typename Traits>
    bool base_node<Traits>::empty() const { return holds_value_ == holds_value_t::empty; }

    template <typename Traits>
    bool base_node<Traits>::has_children() const { return holds_value_ == holds_value_t::child && !children_.empty(); }

    template <typename Traits>
    bool base_node<Traits>::has_value() const { return holds_value_ == holds_value_t::value && value_.has_value(); }

    template <typename Traits>
    const typename Traits::key_t& base_node<Traits>::key() const { return key_; }

    template <typename Traits>
    typename Traits::node_ptr base_node<Traits>::parent() const { return parent_; }

    template <typename Traits>
    const typename Traits::value_t& base_node<Traits>::value() const
    {
        if (!has_value()) throw base_node_error(base_node_error::error_type::value_not_exist, "value does not exits");

        return value_.value();
    }

    template <typename Traits>
    const typename Traits::value_t& base_node<Traits>::value(const typename Traits::value_t& default_value) const
    {
        return has_value() ? value_.value() : default_value;
    }

    template <typename Traits>
    const typename Traits::children_t& base_node<Traits>::children() const
    {
        if (!has_children())
            throw base_node_error(base_node_error::error_type::children_not_exist,
                    "children do not exists");

        return children_;
    }

    template <typename Traits>
    const typename Traits::children_t& base_node<Traits>::children(const typename Traits::children_t& default_value) const
    {
        return has_children() ? children_ : default_value;
    }

    template <typename Traits>
    typename Traits::node_ptr base_node<Traits>::child(const typename Traits::key_t& key)
    {
        if (auto it = children_.find(key); it != children_.cend())
        {
            return it->second;
        }
        return nullptr;
    }

    template <typename Traits>
    typename Traits::node_ptr base_node<Traits>::root()
    {
        return new base_node<Traits>(typename Traits::key_t());
    }

} //namespace

#endif //NODE_HPP