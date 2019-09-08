#ifndef NODE_HPP
#define NODE_HPP

// internal
#include "base_node.h"
#include "base_node_error.h"

// std
#include <algorithm>

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
    bool base_node<Traits>::set_value(const typename Traits::value_t& value)
    {
        bool was_set = false; // use NRVO optimization
        if (holds_value_ != holds_value_t::child)
        {
            value_.reset();
            value_.emplace(value);
            holds_value_ = holds_value_t::value;
            was_set = true;
        }

        return was_set;
    }

    template <typename Traits>
    typename Traits::node_ptr base_node<Traits>::add_child(const typename Traits::key_t& key)
    {
        typename Traits::node_ptr node = nullptr; // use NRVO optimization
        if (holds_value_ != holds_value_t::value)
        {
            node = create_node(key);
            if (node)
            {
                holds_value_ = holds_value_t::child;
            }
        }

        return node;
    }

    template <typename Traits>
    typename Traits::node_ptr base_node<Traits>::add_child
            (const typename Traits::key_t& key, const typename Traits::value_t& value)
    {
        typename Traits::node_ptr node = nullptr; // use NRVO optimization
        if (holds_value_ != holds_value_t::value)
        {
            node = create_node(key);
            if (node)
            {
                node->set_value(value);
                holds_value_ = holds_value_t::child;
            }
        }
        return node;
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
    bool base_node<Traits>::exist(const typename Traits::node_ptr child) const
    {
        auto predicate = [&](const auto& pair) { return pair.second == child; };
        return std::find_if(children_.cbegin(), children_.cend(), predicate) != children_.cend();
    }

    template <typename Traits>
    bool base_node<Traits>::exist(const typename Traits::key_t& key) const
    {
        return children_.find(key) != children_.cend();
    }

    template <typename Traits>
    void base_node<Traits>::clear()
    {
    }

    template <typename Traits>
    void base_node<Traits>::erase(const typename Traits::key_t& key)
    {
    }

    template <typename Traits>
    typename Traits::node_ptr base_node<Traits>::root()
    {
        return new base_node<Traits>(typename Traits::key_t());
    }

    template <typename Traits>
    typename Traits::node_ptr base_node<Traits>::create_node(const typename Traits::key_t& key)
    {
        typename Traits::node_ptr node = nullptr;
        if (!exist(key))
        {
            node = new base_node<Traits>(key, this);
            children_.insert(std::pair<typename Traits::key_t, typename Traits::node_ptr>(key, node));
        }
        return node;
    }

} //namespace

#endif //NODE_HPP