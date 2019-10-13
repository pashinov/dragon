#ifndef BASE_READ_H
#define BASE_READ_H

namespace ptree
{
    template <typename Traits>
    class base_read
    {
    public:
        base_read(typename Traits::node_ptr node);
        virtual ~base_read() = default;

    protected:
        typename Traits::node_ptr node_;
    };

} // namespace

#endif // BASE_READ_H

#include "base_read.hpp"
