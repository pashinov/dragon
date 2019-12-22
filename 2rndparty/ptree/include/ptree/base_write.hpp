#ifndef BASE_WRITE_HPP
#define BASE_WRITE_HPP

// std
#include <type_traits>
#include <optional>

namespace ptree
{
    template <typename Traits>
    base_write<Traits>::base_write(const typename Traits::node_weak_ptr& node) : node_(node) { }

    template <typename Traits>
    void base_write<Traits>::clear() { node_.lock()->clear(); }

    template <typename Traits>
    void base_write<Traits>::erase(const typename Traits::key_t& key) { node_.lock()->erase(key); }

    template <typename Traits>
    bool base_write<Traits>::set_value(const typename Traits::value_t& value)
    {
        return node_.lock()->set_value(value);
    }

    template <typename Traits>
    template <typename Value>
    base_write<Traits>& base_write<Traits>::operator=(const Value& value)
    {
        if (!node_.lock()->has_value())
        {
            node_.lock()->clear();
        }

        // operator=(const base_write<Traits>& )
        if constexpr(std::is_base_of_v<base_write<Traits>, Value>)
        {
            return operator =(static_cast<base_write<Traits>&>(value));
        }
        // operator=(const typename Traits::value_t&)
        else if constexpr(std::is_constructible_v<typename Traits::value_t, Value>)
        {
            this->set_value(value);
        }
        // operator=(std::nullopt)
        else if constexpr(std::is_constructible_v<std::nullopt, Value>)
        {
            this->clear();
        }
        else
        {
            static_assert(!std::is_same_v<Value, Value>, "assignment works only for Value");
        }
        return *this;
    }
}

#endif // BASE_WRITE_HPP
