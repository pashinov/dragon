#ifndef PTREE_CONST_NODE_HPP
#define PTREE_CONST_NODE_HPP

namespace ptree
{
    template <typename Traits>
    ptree_const_node<Traits>::ptree_const_node() : ptree_const_node<Traits>(Traits::root(), true)
    {
//        is_root_ = true;
    }

    template <typename Traits>
    ptree_const_node<Traits>::ptree_const_node(typename Traits::node_ptr node, bool is_root)
        : base_tree<Traits, ptree_const_node<Traits> >(node, is_root),
          base_read<Traits>(node)
//          node_(node)
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
//        if (common_tree_t::is_root()) delete node_;
    }

//    template <typename Traits>
//    bool ptree_const_node<Traits>::is_root() const { return is_root_; }

} // namespace

#endif // PTREE_CONST_NODE_HPP
