#ifndef PTREE_NODE_H
#define PTREE_NODE_H

#include "base_tree.h"

namespace ptree
{
    template <typename Traits>
    class ptree_node : public base_tree<Traits, ptree_node<Traits> >
    {
        friend class base_tree<Traits, ptree_node<Traits> >;
    public:
        ptree_node();
        ~ptree_node();

    private:
        ptree_node(typename Traits::node_ptr node);

    private:
        typename Traits::node_ptr node_;
    };

} // namespace

#endif // PTREE_NODE_H

#include "ptree_node.hpp"