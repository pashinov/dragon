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
        ~base_node();

        bool set_value(const typename Traits::value_t& value);
        
        typename Traits::node_shared_ptr add_child(const typename Traits::key_t& key);
        typename Traits::node_shared_ptr add_child(const typename Traits::key_t& key,
                                                   const typename Traits::value_t& value);

        [[nodiscard]] bool empty() const;
        [[nodiscard]] bool has_children() const;
        [[nodiscard]] bool has_value() const;

        const typename Traits::key_t& key() const;
        typename Traits::node_weak_ptr parent() const;

        const typename Traits::optional_value_t& value() const;

        const std::map<typename Traits::key_t, typename Traits::node_shared_ptr>& children() const;
        const std::map<typename Traits::key_t, typename Traits::node_shared_ptr>& children
            (const std::map<typename Traits::key_t, typename Traits::node_shared_ptr>& default_value) const;

        typename Traits::node_shared_ptr child(const typename Traits::key_t& key) const;

        bool exist(const typename Traits::node_shared_ptr& child) const;
        bool exist(const typename Traits::key_t& child) const;

        void value_reset();
        void clear();
        void erase(const typename Traits::key_t& key);

        base_node(const base_node& other) = delete;
        base_node& operator=(const base_node& other) = delete;

        base_node(base_node&& other) noexcept = default;
        base_node& operator=(base_node&& other) noexcept = default;

        const notification_object<typename Traits::optional_value_t>& value_changed() const;
        const notification_object<typename Traits::key_t>& child_added() const;
        const notification_object<typename Traits::key_t>& child_removed() const;

        static typename Traits::node_shared_ptr root();

    protected:
        explicit base_node(const typename Traits::key_t& key,
                           const std::weak_ptr<typename Traits::node_t>& parent
                                = std::weak_ptr<typename Traits::node_t>());

    private:
        enum class holds_value_t : std::uint8_t { empty, value, child };

        typename Traits::node_shared_ptr create_child(const typename Traits::key_t& key);
        typename Traits::node_shared_ptr create_child(const typename Traits::key_t& key,
                                                      const typename Traits::value_t& value);

    private:
        // weak_ptr to this pointer
        typename Traits::node_weak_ptr weak_this_      { };

        typename Traits::key_t key_                    { typename Traits::key_t() };
        typename Traits::optional_value_t value_       { std::nullopt };
        typename Traits::node_weak_ptr parent_         { nullptr };

        std::map<typename Traits::key_t,
                 typename Traits::node_shared_ptr> children_  { };

        holds_value_t holds_value_                     { holds_value_t::empty };

        notification_object<typename Traits::optional_value_t> value_changed_;
        notification_object<typename Traits::key_t> child_added_;
        notification_object<typename Traits::key_t> child_removed_;
    };

} //namespace

#endif // BASE_NODE_H

#include "base_node.hpp"
