#ifndef BASE_WRITE_H
#define BASE_WRITE_H

namespace ptree
{
    template <typename Traits>
    class base_write
    {
    public:
        base_write(typename Traits::node_ptr node);
        virtual ~base_write() = default;

        void set_value(const typename Traits::value_t& value);

        void clear();
        void erase(const typename Traits::key_t& key);

        template <typename Value>
        base_write& operator=(const Value& value);

    private:
        typename Traits::node_ptr node_;
    };

} // namespace

#endif // BASE_WRITE_H

#include "base_write.hpp"
