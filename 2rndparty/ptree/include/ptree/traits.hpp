#ifndef PTREE_TRAITS_HPP
#define PTREE_TRAITS_HPP

// std
#include <memory>

namespace ptree
{
    template <typename Key, typename Value>
    typename traits<Key, Value>::node_shared_ptr traits<Key, Value>::root()
    {
        return traits<Key, Value>::node_t::root();
    }

    template <typename Key, typename Value>
    typename traits<Key, Value>::node_shared_ptr traits<Key, Value>::get_shared
                    (const traits<Key,Value>::node_weak_ptr& node)
    {
        auto res = node.lock();
        if (!res.get()) throw std::bad_weak_ptr();

        return res;
    }

} // namespace

#endif // PTREE_TRAITS_HPP
