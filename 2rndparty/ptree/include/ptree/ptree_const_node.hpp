#ifndef PTREE_CONST_NODE_HPP
#define PTREE_CONST_NODE_HPP

namespace ptree
{
    template <typename Traits>
    ptree_const_node<Traits>::ptree_const_node() : ptree_const_node<Traits>(Traits::root())
    {
    }

    template <typename Traits>
    ptree_const_node<Traits>::ptree_const_node(const typename Traits::node_shared_ptr& node)
        : base_tree<Traits, ptree_const_node<Traits> >(node),
          base_read<Traits>(node)
    {
    }

//    template <typename Traits>
//    ptree_const_node<Traits>::ptree_const_node(const ptree_node<Traits>& other)
//        : ptree_const_node(other.node_)
//    {
//        is_root_ = other.is_root();
//    }

//    template <typename Traits>
//    template <typename Other>
//    ptree_const_node<Traits>::ptree_const_node(const Other& other)
//        : base_tree<Traits, ptree_const_node<Traits> >(other)
//          base_read<Traits>(other)
//          node_(other.node_)
//    {
//    }

    template <typename Traits>
    ptree_const_node<Traits>::~ptree_const_node()
    {
    }

} // namespace

#endif // PTREE_CONST_NODE_HPP
