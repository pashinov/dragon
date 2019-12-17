#ifndef BASE_READ_H
#define BASE_READ_H

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

    protected:
        explicit base_read(typename Traits::node_ptr node);

        ~base_read() = default;

    private:
        typename Traits::node_ptr node_;
    };

} // namespace

#endif // BASE_READ_H

#include "base_read.hpp"
