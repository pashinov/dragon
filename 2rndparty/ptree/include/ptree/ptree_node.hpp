#ifndef PTREE_NODE_HPP
#define PTREE_NODE_HPP


namespace ptree
{
    template <typename Traits>
    ptree_node<Traits>::ptree_node() : ptree_node(Traits::root())
    {
//        is_root_ = true;
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
    }

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

} // namespace

#endif // PTREE_NODE_HPP
