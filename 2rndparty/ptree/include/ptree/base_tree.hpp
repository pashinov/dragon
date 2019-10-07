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

    template <typename Traits, typename Node>
    std::map<typename Traits::key_t, Node> base_tree<Traits, Node>::children() const
    {
        std::map<typename Traits::key_t, Node> res;
        const auto& node_children = this->node_->children();
        for (auto it = node_children.cbegin(); it != node_children.cend(); ++it)
        {
            res.insert(std::make_pair(it->first, Node(it->second)));
        }
        return res;
    }

    template <typename Traits, typename Node>
    template <typename Path>
    Node base_tree<Traits, Node>::child(const Path& path) const
    {
        // Here using SFINAE in complie time

        // if path type equal typename Traits::key_t
        if constexpr (std::is_constructible_v<typename Traits::key_t, Path>)
        {
            return this->child(typename Traits::key_t(path));
        }
        // if path value type equal typename Traits::key_t and Path have const_iterator
        else if constexpr (std::is_constructible_v<typename Traits::key_t, typename Path::value_type> &&
                           std::is_same_v<typename Path::const_iterator, typename Path::const_iterator>)
        {
            // recursive find node by path part (node key)
            Node node = static_cast<const Node&>(*this);
            for(auto it = path.cbegin(); it != path.cend(); ++it)
            {
                node = node.child(*it);
            }
            return node;
        }
        else
        {
            static_assert(!std::is_same_v<Path, Path>, "Path will be iteratable container to Path-convertible objects");
        }
    }
}

#endif // BASE_TREE_HPP
