#ifndef BASE_TREE_H
#define BASE_TREE_H

// std
#include <map>
#include <iostream>

namespace ptree
{
    // this class should be used in the CRTP pattern
    template <typename Traits, typename Node>
    class base_tree
    {
        // friend class Traits::node_t;
        template <typename, typename>friend class base_tree;
    public:
        [[nodiscard]] bool has_children() const;
        [[nodiscard]] bool exist(const typename Traits::key_t& key) const;
        [[nodiscard]] Node child(const typename Traits::key_t& key) const;

        [[nodiscard]] std::map<typename Traits::key_t, Node> children() const;

        template <typename Path>
        [[nodiscard]] Node child(const Path& path) const;

        template <typename Path>
        [[nodiscard]] Node operator[](const Path& path) const;

        template <typename Other>
        base_tree(const base_tree<Traits, Other>& other);

        base_tree(const base_tree& other) = default;
        base_tree(base_tree&& other) = default;

        bool operator == (const base_tree& other) const;
        bool operator != (const base_tree& other) const;

        base_tree& operator=(const base_tree& other) = default;
        base_tree& operator=(base_tree&& other) = default;

    protected:
        explicit base_tree(const typename Traits::node_shared_ptr& node);
        ~base_tree() { }
    private:
        typename Traits::node_shared_ptr node_;
    };

} // namespace

#endif // BASE_TREE_H

#include "base_tree.hpp"
