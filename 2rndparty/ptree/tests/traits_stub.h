#ifndef TRAITS_STUB_H
#define TRAITS_STUB_H

// std
#include <variant>
#include <string>

class node_mock
{
public:

};

class traits_stub
{
public:
    using node_t = node_mock;
    using node_ptr = node_t*;
    using key_t = std::string;
    using value_t = std::variant<std::monostate, std::int64_t, std::uint64_t, std::string, double, float>;
};

#endif // TRAITS_STUB_H
