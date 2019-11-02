#ifndef BASE_READ_H
#define BASE_READ_H

namespace ptree
{
    template <typename Traits>
    class base_read
    {
    public:
        explicit base_read(typename Traits::node_ptr node);
        virtual ~base_read() = default;

        [[nodiscard]] bool empty() const;
        [[nodiscard]] bool has_value() const;

        [[nodiscard]] explicit operator bool() const;

        typename Traits::key_t key() const;
        typename Traits::value_t value() const;

    protected:
        typename Traits::node_ptr node_;
    };

} // namespace

#endif // BASE_READ_H

#include "base_read.hpp"
