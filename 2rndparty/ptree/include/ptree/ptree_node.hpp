#ifndef PTREE_NODE_HPP
#define PTREE_NODE_HPP

#include <iostream>

namespace ptree
{
    template <typename Traits>
    ptree_node<Traits>::ptree_node() : ptree_node(Traits::root())
    {
    }

    template <typename Traits>
    ptree_node<Traits>::ptree_node(const typename Traits::node_shared_ptr& node)
        : base_tree<Traits, ptree_node<Traits> >(node),
          base_read<Traits>(node),
          base_write<Traits>(node),
          base_notification<Traits>(node),
          node_(node) { }

    template <typename Traits>
    ptree_node<Traits>::~ptree_node()
    {
//        std::cout << "~ptree_node()" << std::endl;
    }

    template <typename Traits>
    bool ptree_node<Traits>::operator == (const ptree_node& other) const
    {
        return node_ == other.node_;
    }

    template <typename Traits>
    bool ptree_node<Traits>::operator != (const ptree_node& other) const
    {
        return node_ != other.node_;
    }

} // namespace

#endif // PTREE_NODE_HPP
