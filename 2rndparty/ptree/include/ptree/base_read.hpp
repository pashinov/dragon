#ifndef BASE_READ_HPP
#define BASE_READ_HPP

namespace ptree
{
    template <typename Traits>
    base_read<Traits>::base_read(typename Traits::node_ptr node) : node_(node) { }

} // namespace

#endif // BASE_READP_HPP
