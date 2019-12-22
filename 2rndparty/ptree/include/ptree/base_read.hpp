#ifndef BASE_READ_HPP
#define BASE_READ_HPP

#include <memory>

namespace ptree
{
    template <typename Traits>
    base_read<Traits>::base_read(const typename Traits::node_weak_ptr& node) : node_(node) { }

    template <typename Traits>
    bool base_read<Traits>::empty() const
    {
        return Traits::get_shared(node_)->empty();
    }

    template <typename Traits>
    bool base_read<Traits>::has_value() const
    {
        return Traits::get_shared(node_)->has_value();
    }

    template <typename Traits>
    base_read<Traits>::operator bool() const
    {
        return !Traits::get_shared(node_)->empty();
    }

    template <typename Traits>
    typename Traits::key_t base_read<Traits>::key() const
    {
        return Traits::get_shared(node_)->key();
    }

    template <typename Traits>
    const typename Traits::optional_value_t& base_read<Traits>::value() const
    {
        return Traits::get_shared(node_)->value();
    }

} // namespace

#endif // BASE_READP_HPP
