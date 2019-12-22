#ifndef NODE_HPP
#define NODE_HPP

// internal
#include "base_node.h"
#include "base_node_error.h"

// std
#include <algorithm>
#include <iostream>

namespace ptree
{
    template <typename Traits>
    base_node<Traits>::base_node(const typename Traits::key_t& key,
                                 const std::weak_ptr<typename Traits::node_t>& parent)
        : key_(key), parent_(parent)
    {
        std::cout << "base_node<Traits>::base_node(key, parent)" << std::endl;
    }

    template <typename Traits>
    base_node<Traits>::~base_node()
    {
        std::cout << "base_node<Traits>::~base_node()" << std::endl;
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
            value_changed_.notify(value_.value());
            was_set = true;
        }

        return was_set;
    }

    template <typename Traits>
    typename Traits::node_weak_ptr base_node<Traits>::add_child(const typename Traits::key_t& key)
    {
        typename Traits::node_shared_ptr node = nullptr; // use NRVO optimization
        if (holds_value_ != holds_value_t::value)
        {
            node = this->create_child(key);
            if (node)
            {
                holds_value_ = holds_value_t::child;
                child_added_.notify(node->key());
            }
        }

        return node;
    }

    // TODO: for return value can be use return_type<V,E>
    template <typename Traits>
    typename Traits::node_weak_ptr base_node<Traits>::add_child(const typename Traits::key_t& key,
                                                                const typename Traits::value_t& value)
    {
        typename Traits::node_shared_ptr node = nullptr; // use NRVO optimization
        if (holds_value_ != holds_value_t::value)
        {
            node = this->create_child(key, value);
            if (node)
            {
                holds_value_ = holds_value_t::child;
                child_added_.notify(node->key());
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
    typename Traits::node_weak_ptr base_node<Traits>::parent() const { return parent_; }

    template <typename Traits>
    const typename Traits::optional_value_t& base_node<Traits>::value() const { return value_; }

    template <typename Traits>
    const std::map<typename Traits::key_t, typename Traits::node_shared_ptr>& base_node<Traits>::children() const
    {
        if (!has_children())
            throw base_node_error(base_node_error::error_type::children_not_exist,
                    "children do not exists");

        return children_;
    }

    template <typename Traits>
    const std::map<typename Traits::key_t, typename Traits::node_shared_ptr>& base_node<Traits>::children
    (const std::map<typename Traits::key_t, typename Traits::node_shared_ptr>& default_value) const
    {
        return has_children() ? children_ : default_value;
    }

    template <typename Traits>
    typename Traits::node_weak_ptr base_node<Traits>::child(const typename Traits::key_t& key) const
    {
        if (auto it = children_.find(key); it != children_.cend())
        {
            return it->second;
        }
        return typename Traits::node_weak_ptr();
    }

    template <typename Traits>
    bool base_node<Traits>::exist(const typename Traits::node_shared_ptr& child) const
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
    void base_node<Traits>::value_reset()
    {
        if (holds_value_ == holds_value_t::value) value_.reset();
    }

    template <typename Traits>
    void base_node<Traits>::clear()
    {
        switch (holds_value_)
        {
        case holds_value_t::value:
        {
            value_.reset();
            holds_value_ = holds_value_t::empty;
            break;
        }
        case holds_value_t::child:
        {
            for (auto it = children_.begin(); it != children_.end();)
            {
                auto node = it->second;
                it = children_.erase(it);
                // if (node) delete node;
            }
            holds_value_ = holds_value_t::empty;
            break;
        }
        case holds_value_t::empty:
        default:
            break;
        }
    }

    template <typename Traits>
    void base_node<Traits>::erase(const typename Traits::key_t& key)
    {
        if (auto it = children_.find(key); it != children_.cend())
        {
            it = children_.erase(it);
            child_removed_.notify(key);
        }
    }

    template <typename Traits>
    const notification_object<typename Traits::optional_value_t>& base_node<Traits>::value_changed() const { return value_changed_; }

    template <typename Traits>
    const notification_object<typename Traits::key_t>& base_node<Traits>::child_added() const { return child_added_; }

    template <typename Traits>
    const notification_object<typename Traits::key_t>& base_node<Traits>::child_removed() const { return child_removed_; }

    template <typename Traits>
    typename Traits::node_shared_ptr base_node<Traits>::root()
    {
        static_assert(std::is_default_constructible<typename Traits::key_t>(),"key_t don't default condtuctible");
        // TODO: use std::make_shared
        typename Traits::node_shared_ptr node(new typename Traits::node_t(typename Traits::key_t()));
        node->weak_this_ = node;
        return node;
    }

    template <typename Traits>
    typename Traits::node_shared_ptr base_node<Traits>::create_child(const typename Traits::key_t& key)
    {
        // TODO: use std::lower_baund
        if (exist(key)) return nullptr;

        // TODO: use std::make_shared
        typename Traits::node_shared_ptr node(new base_node<Traits>(key, weak_this_));
        if (!node) return nullptr;

        node->weak_this_ = node;

        children_.emplace(key, node);

        return node;
    }

    template <typename Traits>
    typename Traits::node_shared_ptr base_node<Traits>::create_child(const typename Traits::key_t& key,
                                                                     const typename Traits::value_t& value)
    {
        if (exist(key)) return nullptr;

        // TODO: use std::make_shared
        typename Traits::node_shared_ptr node(new base_node<Traits>(key, weak_this_));
        if (!node) return nullptr;

        node->weak_this_ = node;
        node->set_value(value);

        children_.emplace(key, node);

        return node;
    }

} //namespace

#endif //NODE_HPP
