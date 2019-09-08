#ifndef PTREE_NODE_HPP
#define PTREE_NODE_HPP

namespace ptree
{
    template <typename Traits>
    ptree_node<Traits>::ptree_node() : ptree_node<Traits>(Traits::root()) { }

    template <typename Traits>
    ptree_node<Traits>::ptree_node(typename Traits::node_ptr node)
        : base_tree<Traits, ptree_node<Traits> >(node), node_(node) { }

    template <typename Traits>
    ptree_node<Traits>::~ptree_node()
    {
        delete node_;
    }

} // namespace

#endif // PTREE_NODE_HPP
