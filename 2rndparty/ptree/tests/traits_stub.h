#ifndef TRAITS_STUB_H
#define TRAITS_STUB_H

// gmock
#include <gmock/gmock.h>

// std
#include <variant>
#include <string>
#include <map>

// forward declaration
class node_mock;

class traits_stub
{
public:
    using node_t = node_mock;
    using node_ptr = node_t*;
    using key_t = std::string;
    using value_t = std::variant<std::monostate, std::int64_t, std::uint64_t, std::string, double, float>;
};

class node_mock
{
public:
    MOCK_CONST_METHOD0(has_children, bool());
    MOCK_CONST_METHOD1(exist, bool(const traits_stub::key_t&));
    MOCK_CONST_METHOD1(child, traits_stub::node_ptr(const traits_stub::key_t&));
    MOCK_METHOD1(add_child, traits_stub::node_ptr(const traits_stub::key_t&));
    MOCK_CONST_METHOD0(children, std::map<traits_stub::key_t, traits_stub::node_ptr>());
};

#endif // TRAITS_STUB_H
