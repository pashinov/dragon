#ifndef BASE_TREE_H
#define BASE_TREE_H

namespace ptree
{
    template <typename Traits, typename Node>
    class base_tree
    {
    public:
        base_tree(typename Traits::node_ptr node);
        ~base_tree() = default;

        [[nodiscard]] bool has_children() const;
        [[nodiscard]] Node child(const typename Traits::key_t& key) const;

    protected:
        typename Traits::node_ptr node_;
    };

} // namespace

#endif // BASE_TREE_H

#include "base_tree.hpp"
