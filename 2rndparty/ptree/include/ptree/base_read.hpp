#ifndef BASE_READ_HPP
#define BASE_READ_HPP

namespace ptree
{
    template <typename Traits>
    base_read<Traits>::base_read(typename Traits::node_ptr node) : node_(node) { }

    template <typename Traits>
    bool base_read<Traits>::empty() const { return node_->empty(); }

    template <typename Traits>
    bool base_read<Traits>::has_value() const { return node_->has_value(); }

    template <typename Traits>
    base_read<Traits>::operator bool() const
    {
        return !node_->empty();
    }

} // namespace

#endif // BASE_READP_HPP
