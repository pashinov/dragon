#ifndef BASE_WRITE_H
#define BASE_WRITE_H

namespace ptree
{
    template <typename Traits>
    class base_write
    {
    public:
        bool set_value(const typename Traits::value_t& value);

        void clear();
        void erase(const typename Traits::key_t& key);

        base_write& operator=(const base_write& other) = default;

        template <typename Value>
        base_write& operator=(const Value& value);

    protected:
        base_write(const typename Traits::node_weak_ptr& node);
        ~base_write() = default;

    private:
        typename Traits::node_weak_ptr node_;
    };

} // namespace

#endif // BASE_WRITE_H

#include "base_write.hpp"
