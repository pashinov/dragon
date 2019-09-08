#ifndef TRAITS_HPP
#define TRAITS_HPP

namespace ptree
{
    template <typename Key, typename Value>
    typename traits<Key, Value>::node_ptr traits<Key, Value>::root()
    {
        return traits<Key, Value>::node_t::root();
    }

} // namespace

#endif // TRAITS_HPP
