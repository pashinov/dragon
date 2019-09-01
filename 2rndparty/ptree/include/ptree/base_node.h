#ifndef BASE_NODE_H
#define BASE_NODE_H

// internal
#include "traits.h"

// std
#include <optional>

namespace ptree
{
    template <typename Traits>
    class base_node
    {
    public:
        using base_node_ptr = base_node*;

        base_node(const typename Traits::key_t& key, base_node_ptr parent = nullptr);
        ~base_node() = default;

        const typename Traits::key_t& key() const;
        typename Traits::value_t value() const;
        typename Traits::children_t& children() const;
        base_node* child(const typename Traits::key_t& key);
    private:
        enum class holds_value_t : std::uint8_t { value, child };
    private:
        typename Traits::key_t key_;
        std::optional<typename Traits::value_t> value_;
        base_node* parent_;
        typename Traits::children_t children_;
        holds_value_t holds_value_;
    };

} //namespace

#endif // BASE_NODE_H

#include "base_node.hpp"