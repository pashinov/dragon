#ifndef PTREE_CONST_NODE_H
#define PTREE_CONST_NODE_H

// internal
#include "base_tree.h"
#include "base_read.h"

namespace ptree
{
    template <typename Traits>
    class ptree_const_node : public base_tree<Traits, ptree_const_node<Traits> >,
                             public base_read<Traits>
    {
        friend class base_tree<Traits, ptree_const_node<Traits> >;

        using common_tree_t = base_tree<Traits, ptree_const_node<Traits> >;
        using reading_tree_t = base_read<Traits>;

    public:
        ptree_const_node();
        ~ptree_const_node();

        // the common base functionality of the tree
        using common_tree_t::has_children;
        using common_tree_t::exist;
        using common_tree_t::child;
        using common_tree_t::children;
        using common_tree_t::operator[];
        using common_tree_t::operator==;

        // the reading functionality of the tree
        using reading_tree_t::empty;
        using reading_tree_t::has_value;
        using reading_tree_t::operator bool;
        using reading_tree_t::key;
        using reading_tree_t::value;

        template <typename Other>
        ptree_const_node(const Other& other)
            : base_tree<Traits, ptree_const_node<Traits> >(other),
              base_read<Traits>(other)
//              node_(other.node_),
//              is_root_(other.is_root_)
        {
        }

        template <typename Other>
        ptree_const_node(Other&& other)
            : base_tree<Traits, ptree_const_node<Traits> >(other),
              base_read<Traits>(other)
        {
        }

//        [[nodiscard]] bool is_root() const;
    private:
        ptree_const_node(typename Traits::node_ptr node, bool is_root = false);

    private:
//        bool is_root_ = false;
//        typename Traits::node_ptr node_ = nullptr;
    };

} // namespace

#endif // PTREE_CONST_NODE_H

#include "ptree_const_node.hpp"
