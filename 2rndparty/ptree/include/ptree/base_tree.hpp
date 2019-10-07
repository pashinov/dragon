#ifndef BASE_TREE_HPP
#define BASE_TREE_HPP

namespace ptree
{
    template <typename Traits, typename Node>
    base_tree<Traits, Node>::base_tree(typename Traits::node_ptr node) : node_(node) { }

    template <typename Traits, typename Node>
    bool base_tree<Traits, Node>::has_children() const
    {
        return this->node_->has_children();
    }

    template <typename Traits, typename Node>
    Node base_tree<Traits, Node>::child(const typename Traits::key_t& key) const
    {
        auto node = this->node_->child(key);
        return node ? node : this->node_->add_child(key);
    }

}

#endif // BASE_TREE_HPP
