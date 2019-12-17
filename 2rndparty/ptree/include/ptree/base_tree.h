#ifndef BASE_TREE_H
#define BASE_TREE_H

// std
#include <map>

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
        base_tree(const base_tree<Traits, Other>& other)
            : node_(other.node_), is_root_(other.is_root_)
        {
        }

        bool is_root() const { return is_root_; }

        bool operator == (const base_tree& other) const;
        bool operator != (const base_tree& other) const;
    protected:
        explicit base_tree(typename Traits::node_ptr node, bool is_root = false);
        ~base_tree()
        {
            if (is_root_)
            {
                delete node_;
            }
        }

    private:
        typename Traits::node_ptr node_;
        bool is_root_;
    };

} // namespace

#endif // BASE_TREE_H

#include "base_tree.hpp"
