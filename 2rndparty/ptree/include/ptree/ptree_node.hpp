#ifndef PTREE_NODE_HPP
#define PTREE_NODE_HPP


namespace ptree
{
    template <typename Traits>
    ptree_node<Traits>::ptree_node() : ptree_node<Traits>(Traits::root(), true)
    {
//        is_root_ = true;
    }

    template <typename Traits>
    ptree_node<Traits>::ptree_node(typename Traits::node_ptr node, bool is_root)
        : base_tree<Traits, ptree_node<Traits> >(node, is_root),
          base_read<Traits>(node),
          base_write<Traits>(node),
          base_notification<Traits>(node) { }
//          node_(node) { }

    template <typename Traits>
    ptree_node<Traits>::~ptree_node()
    {
        // if (common_tree_t::is_root()) delete node_;
    }

//    template <typename Traits>
//    bool ptree_node<Traits>::is_root() const { return is_root_; }

    template <typename Traits>
    bool ptree_node<Traits>::operator == (const ptree_node& other) const
    {
        return common_tree_t::operator==(other);
    }

    template <typename Traits>
    bool ptree_node<Traits>::operator != (const ptree_node& other) const
    {
        return common_tree_t::operator != (other);
    }

//    template <typename Traits>
//    ptree_const_node<Traits> ptree_node<Traits>::to_ptree_const_node() const
//    {
//        // return ptree_const_node<Traits>(node_);
//    }

} // namespace

#endif // PTREE_NODE_HPP
