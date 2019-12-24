#ifndef BASE_WRITE_H
#define BASE_WRITE_H

#include <iostream>

namespace ptree
{
    template <typename Traits>
    class base_write
    {
    public:
        [[nodiscard]] bool set_value(const typename Traits::value_t& value);

        void clear();
        void erase(const typename Traits::key_t& key);

        base_write& operator=(const base_write& other) = default;

        template <typename Value>
        base_write& operator=(const Value& value);

    protected:
        base_write(const typename Traits::node_shared_ptr& node);
        ~base_write() { }
    private:
        typename Traits::node_shared_ptr node_;
    };

} // namespace

#endif // BASE_WRITE_H

#include "base_write.hpp"
