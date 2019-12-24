#ifndef BASE_READ_H
#define BASE_READ_H

#include <iostream>

namespace ptree
{
    template <typename Traits>
    class base_read
    {
    public:
        [[nodiscard]] bool empty() const;
        [[nodiscard]] bool has_value() const;

        [[nodiscard]] explicit operator bool() const;

        typename Traits::key_t key() const;
        const typename Traits::optional_value_t& value() const;

        base_read(const base_read<Traits>& other) = default;
        base_read& operator=(const base_read& other) = default;

        base_read(base_read<Traits>&& other) = default;
        base_read& operator=(base_read&& other) = default;

    protected:
        explicit base_read(const typename Traits::node_shared_ptr& node);
        ~base_read() { }

    private:
        typename Traits::node_shared_ptr node_;
    };

} // namespace

#endif // BASE_READ_H

#include "base_read.hpp"
