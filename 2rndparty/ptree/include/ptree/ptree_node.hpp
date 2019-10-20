#ifndef PTREE_NODE_HPP
#define PTREE_NODE_HPP

namespace ptree
{
    template <typename Traits>
    ptree_node<Traits>::ptree_node() : ptree_node<Traits>(Traits::root())
    {
        is_root_ = true;
    }

    template <typename Traits>
    ptree_node<Traits>::ptree_node(typename Traits::node_ptr node)
        : base_tree<Traits, ptree_node<Traits> >(node),
          base_read<Traits>(node),
          node_(node) { }

    template <typename Traits>
    ptree_node<Traits>::~ptree_node()
    {
        if (this->is_root()) delete node_;
    }

    template <typename Traits>
    bool ptree_node<Traits>::is_root() const { return is_root_; }

} // namespace

#endif // PTREE_NODE_HPP
