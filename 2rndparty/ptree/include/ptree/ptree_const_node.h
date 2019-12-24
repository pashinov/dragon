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
        {
        }

        template <typename Other>
        ptree_const_node(Other&& other)
            : base_tree<Traits, ptree_const_node<Traits> >(other),
              base_read<Traits>(other)
        {
        }
    private:
        ptree_const_node(const typename Traits::node_shared_ptr& node);
    };

} // namespace

#endif // PTREE_CONST_NODE_H

#include "ptree_const_node.hpp"
